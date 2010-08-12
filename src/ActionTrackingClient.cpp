/*! ----------------------------------------------------
 *  Copyright 2010, CNRS-AIST JRL
 *
 * ---------------------------------------------------- */

#include <iostream>

#include "LLVC/ActionTrackingClient.h"
#include "LowLevelVisionSystem.hh"


namespace trackingClient
{
  void convertCorbaImageToVispImage(const ImageData_var& imageCorbaSrc,
				    vpImage<unsigned char>& imageVispDest);


  ActionTrackingClient::ActionTrackingClient(const std::string& processName)
    : LLVClient::ActionWithLLVSBase(),
      m_serverProcessName(processName),
      m_LLVS(GetServicePort<LowLevelVisionSystem,
	     LowLevelVisionSystem_var>()),
      m_paramList(),
      m_image(),
      m_cameraID(0),
      m_format("RGB_VISPU8_NONE")
  {
    HRP2CameraSeq_var cameraSeq;
    m_LLVS->getCameraSeq(cameraSeq);
    HRP2Camera_var camera = cameraSeq[m_cameraID];
    HRP2Camera::CameraParameter_var cameraParameter =
      camera->GetCameraParameter();


    long imWidth = cameraParameter->Width;
    long imHeight = cameraParameter->Height;
    m_image.resize(imHeight,imWidth);

    m_LLVS->SetSynchronizationMode(LowLevelVisionSystem::SYNCHRO_TRIGGER);

    m_LLVS->SetAProcessParameterAndValue
      (CORBA::string_dup("IEEE1394 Image grabbing"),
       CORBA::string_dup("Format"),
       CORBA::string_dup("RGB"));
    m_LLVS->SetAProcessParameterAndValue
      (CORBA::string_dup("vispUndistordedProcess"),
       CORBA::string_dup("TYPE_CONV"),
       CORBA::string_dup(m_format.c_str()));

    m_LLVS->StartProcess("IEEE1394 Image grabbing");
    m_LLVS->StartProcess("vispUndistordedProcess");
  }

  ActionTrackingClient::~ActionTrackingClient()
  {
    m_LLVS->StopProcess("IEEE1394 Image grabbing");
    m_LLVS->StopProcess("vispUndistordedProcess");
  }

  std::ostream&
  ActionTrackingClient::print (std::ostream& stream) const
  {
    stream << "ActionTrackingClient:" << std::endl
	   << "\t process name: " << m_serverProcessName << std::endl
	   << "\t camera id: " << m_cameraID << std::endl;
    return stream;
  }

  void ActionTrackingClient::setTrackingParameters(std::string paramName,
						   std::string paramValue)
  {
    m_LLVS->SetAProcessParameterAndValue(m_serverProcessName.c_str (),
					 paramName.c_str (),
					 paramValue.c_str ());
    impl_setTrackingParameters(paramName, paramValue);
  }

  void
  ActionTrackingClient::grabImageServer()
  {
    ImageData_var corbaImage;
    m_LLVS->TriggerSynchro();

    CORBA::Long cameraId(m_cameraID);

    m_LLVS->getRectifiedImage(cameraId, corbaImage);
    convertCorbaImageToVispImage(corbaImage, m_image);
  }

  std::ostream& operator <<(std::ostream& stream,
			    const ActionTrackingClient& actionTrackingClient)
  {
    return actionTrackingClient.print (stream);
  }

} // end of namespace trackingClient
