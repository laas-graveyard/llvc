// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include <fstream>

#include <boost/format.hpp>

#include <visp/vpImageIo.h>

/// this include is to have the function getInitFileFromModelName
#include "llvc/action-display-mbt.h"
#include "llvc/action-display-with-command.h"
#include "llvc/action-tracking-with-command.h"
#include "llvc/tools/indent.hh"

// LLVC_LOGGINGDIR is used to know where to log debug information.
#ifndef LLVC_LOGGINGDIR
# error "LLVC_LOGGINGDIR should be defined."
#endif //! LLVC_LOGGINGDIR

namespace trackingClient
{
  /// \brief Logging directory.
  static const std::string loggingDir = LLVC_LOGGINGDIR;

  ActionDisplayWithCommand::ActionDisplayWithCommand
  (boost::shared_ptr<ActionGrab> gc,
   const std::string& modelName,
   const std::string& configurationName,
   unsigned desPoseNb,
   vpColor color,
   bool logData,
   std::string fileNameOfPoses)
    : ActionDisplay(gc),
      m_trackingClient(),
      m_actionTracking(),
      m_tracker(),
      m_initialPose(),
      m_desiredPoseList(),
      m_desPoseNb(desPoseNb),
      m_color(color),
      m_logData(logData),
      m_modelName(modelName),
      m_configurationName(configurationName),
      m_fileNameOfPoses(fileNameOfPoses)
  {
    m_actionGrabClient->ExecuteAction();
    m_image = m_actionGrabClient->image();
    vpDisplay::display(m_image);
    vpDisplay::flush(m_image);

    // set the client tracker parameters
    // warning : if the parameters of the camera are not set,
    // the computation of the pose is false.
    // so we have to set the tracker parameters
    vpCameraParameters cam = m_actionGrabClient->camera();
    m_tracker.setCameraParameters(cam);
    m_tracker.loadModel(getModelFileFromModelName(modelName).c_str());
    
    // Initialize the desired poses to be tracked.
    // Test if a filename is available.
    if (!readFileOfPoses())
      // if not start the user interface.
      userInterfaceInitPoses();


    // Initialise the initial pose
    std::cout << std::endl
	      <<" Please move the robot to the Initial position." 
	      << std::endl;
    std::cout << " Click on the image when ready." 
	      << std::endl;
    waitForUserClick();
    try
      {
	clickToInitPose(m_initialPose);
      }
    catch(...)
      {
	std::cout << "problem in init pose" << std::endl;
      }

    ODEBUG3("Build The Tracker");
    // FIXME : specific to mbt. shoulb be changed to be more generix 
    m_actionTracking = boost::shared_ptr<ActionTracking>
      (new ActionTrackingMbt
       (m_initialPose,
    	m_actionGrabClient,
    	m_modelName,
    	m_configurationName));
    
   
    m_trackingClient = 
      boost::shared_ptr<ActionTrackingWithCommand>
      (new ActionTrackingWithCommand
       (m_actionTracking,
	m_desiredPoseList) );
       
  
 
    // read parameters in config files
    readParameters();
    if (m_logData)
      m_trackingClient->setTrackingParameters("DATA", "ON");

    
    
  }

  ActionDisplayWithCommand::~ActionDisplayWithCommand()
  {
  }
  


  /// wait for a user click
  void  
  ActionDisplayWithCommand::waitForUserClick(std::string text )
  {

    std::cout << text << std::endl;
    bool userHasClicked = false; 
    while (!userHasClicked)
      {
	m_actionGrabClient->ExecuteAction();
	m_image = m_actionGrabClient->image();
	vpDisplay::display(m_image);
	vpDisplay::displayCharString(m_image, 
				 100, 100, 
				 text.c_str(),
				 vpColor::blue);
  
	vpDisplay::flush(m_image);
	userHasClicked = vpDisplay::getClick(m_image,false);
      }

    vpDisplay::display(m_image);
    std::string textYouClicked = "Click Ok ! continue ... ";
    vpDisplay::displayCharString(m_image, 
				 100, 100, 
				 textYouClicked.c_str(),
				 vpColor::red);
    vpDisplay::flush(m_image);
    sleep(1);
    vpDisplay::display(m_image);
    vpDisplay::flush(m_image);
     
     
  }

  bool ActionDisplayWithCommand::readFileOfPoses()
  {
    bool accessFileOfPoses=true;
    
    /* Start predicates */
    if (m_fileNameOfPoses.size()==0)
      {
	
	return false;
      }
    
    /* Opening file */
    std::ifstream aif;
    aif.open((char *)m_fileNameOfPoses.c_str(),
	     std::ifstream::in);

    if (!aif.is_open())
      {
	ODEBUG3("Unable to open " << m_fileNameOfPoses);
	return false;
      }
    // End predicates.


    aif >> m_desPoseNb;
    vpPoseVector lPoseVector;
    m_desiredPoseList.resize(m_desPoseNb);
    for(unsigned int lIndex=0;
	lIndex<m_desPoseNb;
	lIndex++)
      {
	for(unsigned int lpvi=0;
	    lpvi < 6; lpvi++)
	  aif >> lPoseVector[lpvi];
	
	m_desiredPoseList[lIndex].buildFrom(lPoseVector);
      }

    aif.close();
    return accessFileOfPoses;

  }

  void ActionDisplayWithCommand::userInterfaceInitPoses()
  {
    // Initilise the desired pose
    std::cout << " You have to initialise " 
	      << m_desPoseNb 
	      << " desired position(s)" 
	      << std::endl<< std::endl;
    
    for(unsigned int i=0; i<m_desPoseNb;++i)
      {
	std::cout << std::endl
		  << " Please, move the robot to the desired position number "
		  << i+1
		  << std::endl
		  << " Click on the image when ready." 
		  << std::endl;
	waitForUserClick();
        clickToInitDesiredPose(m_desiredPoseList);
      }
    
    std::cout << std::endl
	      << " Would you like to save the points ? (y/n) "
	      << std::endl;
    char lanswer;
    std::cin >> lanswer;
    if ((lanswer=='y') || (lanswer=='Y'))

    std::cout << std::endl
	      << " Please enter the filename where to save the data. "
	      << std::endl;
    std::string lFileOfPoses;
    std::cin >> lFileOfPoses;

    std::ofstream aof;
    aof.open((char *)lFileOfPoses.c_str(),std::ifstream::out);
    aof <<  m_desPoseNb << std::endl;
    for(unsigned int i=0;i<m_desiredPoseList.size();i++)
      {
	vpPoseVector avp;
	avp.buildFrom(m_desiredPoseList[i]);
	for(unsigned int j=0;j<6;j++)
	  aof << avp[j] << " ";
	aof << std::endl;
      }
    aof.close();
  }

  void ActionDisplayWithCommand::readParameters()
  {
    
    m_trackingClient->readParameters();
  }

  bool 
  ActionDisplayWithCommand::Initialize()
  {
    readParameters();
    m_trackingClient->Initialize();
    // FIXME : always true
    return ActionDisplay::Initialize();
  }
    

  void
  ActionDisplayWithCommand::clickToInitDesiredPose
  (homogeneousMatrix_t& desiredPoseList)
  {
    vpHomogeneousMatrix desiredPose;
    clickToInitPose(desiredPose);
    desiredPoseList.push_back(desiredPose);
  
    ODEBUG(" Desired position saved:\n " << std ::endl);
    for (unsigned i = 0 ; i < desiredPoseList.size();++i) 
      ODEBUG(  i  << "---\t"
		<< desiredPoseList[i] 
		<< std::endl);
      
  }

  bool 
  ActionDisplayWithCommand::clickToInitPose(vpHomogeneousMatrix & cMo)
  {
    m_tracker.resetTracker();
    vpCameraParameters cam = m_actionGrabClient->camera();
    m_tracker.setCameraParameters(cam);
    m_tracker.loadModel(getModelFileFromModelName(m_modelName).c_str());
  
    try{
    m_tracker.initClick(m_image, 
			getInitFileFromModelName(m_modelName).c_str());
    
    m_tracker.track(m_image);
    }
    catch(...)
    {
      std::cout << "clickToInitPose Error"<<std::endl;  
      return false;
    }
    m_tracker.getPose(cMo); 
    m_tracker.display(m_image,cMo,cam,vpColor::blue);
    vpDisplay::flush(m_image);
    sleep(1);
    return true;
  }


  bool 
  ActionDisplayWithCommand::ExecuteAction()
  {

    ODEBUG("Execute Action");

    m_trackingClient->ExecuteAction();
    m_image = m_trackingClient->image();

    //FIXME: send patch to enhance ViSP constness.
    // I.e. these copies would not be required if the constness
    // was handled correctly by vpMbtTracker.
    vpCameraParameters cam = m_actionGrabClient->camera();
    vpHomogeneousMatrix cMo = m_trackingClient->pose();

    //FIXME: we do *not* grab using the grabbing
    //client anymore as ActionTrackingMbt::getBufferData
    //is giving us a synchronized image.
    //if (!ActionDisplay::ExecuteAction())
    // return false;
    vpDisplay::display(m_image);
    m_tracker.display (m_image, cMo, cam, m_color);
    vpDisplay::flush(m_image);

    if (m_logData)
      logData();

    return true;
  }

  void 
  ActionDisplayWithCommand::CleanUp()
  {
    m_trackingClient->CleanUp();
    ActionDisplay::CleanUp();
  }

  std::ostream&
  ActionDisplayWithCommand::print (std::ostream& stream) const
  {
    ActionDisplay::print (stream);
    stream << iendl
	   << "ActionDisplayWithCommand:" << incindent << iendl
	   << "tracking nb des pose: " << m_desPoseNb << iendl
	   << "tracking color: " << m_color << iendl
	   << "tracking save log: " << m_logData << iendl
	   << "tracking client:" << incindent << iendl;

    if (m_trackingClient)
      stream << *m_trackingClient;
    else
      stream << "no tracking client";

    stream << decindent << decindent;
    return stream;
  }

  namespace
  {
    std::string makeLogFilename(const std::string& filename)
    {
      std::string result(loggingDir);
      result += "/";
      result += filename;
      return result;
    }
  } // end of namespace gnuplot.

  void
  ActionDisplayWithCommand::logData() const
  {
    static std::ofstream file(makeLogFilename("llvc-mbt.log").c_str());
    static unsigned index = 0;

    if (!file)
      throw "failed to log data";

    if (!index)
      {
	file
	  << "# index (1) | timestamp.first (2) |  timestamp.second (3) | "
	  << " translation (4-6) | rx, ry, rz (7-9)" << std::endl;
      }

    const ActionTrackingMbt::image_t& image = m_trackingClient->image();
    const vpHomogeneousMatrix& cMo = m_trackingClient->pose();
    const ActionTrackingMbt::timestamp_t& timestamp = m_trackingClient->timestamp();

    vpThetaUVector thetaUVector(cMo);
    vpRxyzVector rxyzVector(thetaUVector);

    file
      << index << " "
      << timestamp.first << " "
      << timestamp.second << " "
      << cMo[0][3] << " "
      << cMo[1][3] << " "
      << cMo[2][3] << " "
      << rxyzVector[0] << " "
      << rxyzVector[1] << " "
      << rxyzVector[2] << std::endl;
    ++index;

    // ViSP is using overlays.
    // We have to render the overlay *on* the image
    // before storing it on the disk.
    vpImage<vpRGBa> colorImage;
    vpDisplay::getImage(m_image, colorImage);

    boost::format fmtColor("Ires-%04d.ppm");
    fmtColor % index;

    boost::format fmt("I-%04d.pgm");
    fmt % index;

    vpImageIo::writePPM(colorImage, makeLogFilename(fmtColor.str()));
    vpImageIo::writePGM(image, makeLogFilename(fmt.str()));
  }
} // end of namespace trackingClient
