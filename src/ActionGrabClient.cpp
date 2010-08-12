/*! ----------------------------------------------------
 *  Copyright 2010, CNRS-AIST JRL
 *
 * ---------------------------------------------------- */

#include <iostream>
#include <fstream>
#include "LLVC/ActionGrabClient.h"
#include "LowLevelVisionSystem.hh"


namespace trackingClient
{
  // FIXME: move this elsewhere.
  /// \brief Convert Corba image to ViSP image.
  ///
  /// Throw if the image sizes does not match.
  ///
  /// \param imageCorbaSrc Corba source image
  /// \param imageVispDest ViSP destination image
  void convertCorbaImageToVispImage(const ImageData_var& imageCorbaSrc,
				    vpImage<unsigned char>& imageVispDest)
  {
    std::cout << imageCorbaSrc->width << std::endl
	      << imageVispDest.getWidth() << std::endl;
    if (imageCorbaSrc->width - imageVispDest.getWidth() != 0
	|| imageCorbaSrc->height - imageVispDest.getHeight() != 0)
      throw "Invalid image size in convertCorbaImageToVispImage";

    unsigned char *pt =imageVispDest.bitmap;
    for(unsigned int l=0;l<imageCorbaSrc->octetData.length();++l)
      *pt++ = imageCorbaSrc->octetData[l];
  }


  ActionGrabClient::ActionGrabClient()
    : LLVClient::ActionWithLLVSBase(),
      m_LLVS(GetServicePort<LowLevelVisionSystem,
	     LowLevelVisionSystem_var>()),
      m_image(),
      m_cameraID(3),
      m_format("RGB_VISPU8_NONE")
  {

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
    
    try
      {
	m_LLVS->StartProcess("vispUndistordedProcess");
      }
    catch(...)
      {
	std::cerr<< "Starting the vispUndistortedProcess." << std::endl;
      }

    CORBA::Long lw,lh;
    try 
      {
	m_LLVS->GetImagesGrabbedSize(m_cameraID,lw,lh);
      }
    catch(...)
      {
	std::cerr<< "Unable to GetImagesGrabbedSize LLVS" << std::endl;
      }

    std::cout << "lw: " << lw << "  lh:" << lh << std::endl;
    long imWidth = lw;
    long imHeight = lh;
    m_image.resize(imHeight,imWidth);
  }

  ActionGrabClient::~ActionGrabClient()
  {
    m_LLVS->StopProcess("IEEE1394 Image grabbing");
    m_LLVS->StopProcess("vispUndistordedProcess");
  }

  std::ostream&
  ActionGrabClient::print (std::ostream& stream) const
  {
    stream << "ActionGrabClient:" << std::endl
	   << "\t camera id: " << m_cameraID << std::endl;
    return stream;
  }

  bool
  ActionGrabClient::Initialize()
  {
    return true;
  }

  bool
  ActionGrabClient::ExecuteAction()
  {
    ImageData_var corbaImage;
    m_LLVS->TriggerSynchro();

    CORBA::Long cameraId(m_cameraID);
    CORBA::String_var format = CORBA::string_dup(m_format.c_str());
    sleep(1);
    m_LLVS->getRectifiedImage(cameraId, corbaImage);
  
    convertCorbaImageToVispImage(corbaImage, m_image);
    return true;
  }

  void
  ActionGrabClient::CleanUp()
  {
  }

  std::ostream& operator <<(std::ostream& stream,
			    const ActionGrabClient& actionTrackingClient)
  {
    return actionTrackingClient.print (stream);
  }
} // end of namespace trackingClient
