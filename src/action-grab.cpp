// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include <iostream>
#include <fstream>

#include <visp/vpXmlParserCamera.h>
#include <visp/vpCameraParameters.h>

#include "llvc/action-grab.h"
#include "llvc/tools/indent.hh"
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
    if (imageCorbaSrc->width - imageVispDest.getWidth() != 0
	|| imageCorbaSrc->height - imageVispDest.getHeight() != 0)
      throw "Invalid image size in convertCorbaImageToVispImage";

    unsigned char *pt =imageVispDest.bitmap;
    for(unsigned int l=0;l<imageCorbaSrc->octetData.length();++l)
      *pt++ = imageCorbaSrc->octetData[l];
  }


  ActionGrab::ActionGrab()
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

    long imWidth = lw;
    long imHeight = lh;
    m_image.resize(imHeight,imWidth);

    //---- FIXME : this might be better not to have this written like that
    const std::string camParamPath="./data/ViSP/hrp2CamParam/hrp2.xml";
    const std::string camName="cam1394_3_rectif";
    const vpCameraParameters::vpCameraParametersProjType proj=
      vpCameraParameters::perspectiveProjWithoutDistortion;
    vpXmlParserCamera camParser;
    if(camParser.parse(m_cam,
		      camParamPath.c_str(),
		      camName,
		      proj,
		      imWidth,
		       imHeight)!=vpXmlParserCamera::SEQUENCE_OK)
      throw "Fail to open or parse camera parameter file";
  }

  ActionGrab::~ActionGrab()
  {
    m_LLVS->StopProcess("IEEE1394 Image grabbing");
    m_LLVS->StopProcess("vispUndistordedProcess");
  }

  std::ostream&
  ActionGrab::print (std::ostream& stream) const
  {
    stream << "ActionGrab:" << incindent << iendl
	   << "camera id: " << m_cameraID << iendl
	   << "image format: " << m_format << iendl
	   << "image size: "
	   << m_image.getWidth() << "x" << m_image.getHeight() << decindent;
    return stream;
  }

  bool
  ActionGrab::Initialize()
  {
    return true;
  }

  bool
  ActionGrab::ExecuteAction()
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
  ActionGrab::CleanUp()
  {
  }

  std::ostream& operator <<(std::ostream& stream,
			    const ActionGrab& actionTrackingClient)
  {
    actionTrackingClient.print (stream);
    return stream;
  }
} // end of namespace trackingClient
