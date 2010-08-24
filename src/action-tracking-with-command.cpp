// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include <visp/vpColVector.h>
#include <visp/vpPoseVector.h>

#include "llvc/action-tracking-with-command.h"
#include "llvc/tools/indent.hh"
#include "llvc/tools/visp-io.hh"

namespace trackingClient
{

  // FIXME : this code is duplicated in many classes
  
  /// \brief default data path
  static const std::string defaultPath = "./data/model/";
  /// \brief Installation prefix.
  static const std::string prefix = CMAKE_INSTALL_PREFIX;
  /// \brief Logging directory.
  static const std::string loggingDir = prefix + "/var/log";
  /// \brief Process name
  static const std::string computeLawProcess_name = "ComputeControlLawProcess";

  
  
  std::string getCommandFileFromModelName (const std::string& modelName)
  {
    std::string res(defaultPath);
    res += modelName + "/" + modelName + "-command.conf";
    return res;
  }

  ActionTrackingWithCommand::ActionTrackingWithCommand
  (boost::shared_ptr<ActionTracking> trackerClient,
   homogeneousMatrix_t& desiredPoseList,
   double threshold,
   unsigned currentIndex)
    : ActionTracking(computeLawProcess_name, trackerClient->grabClient()),
      m_trackerClient(trackerClient),
      m_cdMoTab(desiredPoseList),
      m_threshold(threshold),
      m_index(currentIndex)
     
  {
    ODEBUG3(" m_trackerClient pose\n"<< m_trackerClient->pose());
    ModelTrackerInterface_var serverTracker;
    serverTracker = m_LLVS->getModelTracker();
    
    ModelTrackerInterface::HomogeneousMatrix corbacMo,corbacdMo;
    convertViSPHomogeneousMatrixToCorba(pose(),corbacMo);
    convertViSPHomogeneousMatrixToCorba(desiredPose(),corbacdMo);
    serverTracker->SetcMo(corbacMo);
    serverTracker->SetcdMo(corbacdMo);

  }

  ActionTrackingWithCommand::~ActionTrackingWithCommand()
  { 
  }

  std::ostream&
  ActionTrackingWithCommand::print (std::ostream& stream) const
  {
    stream << "ActionTrackingWithCommand:"
	   << iendl
	   << "Nb desired pose: " << m_cdMoTab.size() 
	   << iendl
	   << "Current pose: " << m_index 
	   << iendl
	   << "cMo: "  <<  iendl;
    display (stream, pose());
    stream << "cdMo current: " <<  iendl;
    display (stream, desiredPose());
    stream << "cdMo list: " <<   incindent << iendl;
    for(unsigned i=0; i < m_cdMoTab.size() ; ++i )
      {
	if(i==m_index)
	  stream << "* " ;
	stream << "   pose " << i 
	       << iendl;
	display(stream, m_cdMoTab[i]);
	
      }
    stream << decindent;
    stream << "velocity :" << incindent <<iendl;
    stream << incindent;
    if (m_trackerClient)
      stream << *m_trackerClient;
    else
      stream << "no tracking client";
    return stream;
  }
  
  const vpHomogeneousMatrix&   
  ActionTrackingWithCommand::desiredPose() const
  {
    if (m_index<0 | m_index >= m_cdMoTab.size())
      throw "Invalid current index in ActionTrackingWithCommand";
    return m_cdMoTab[m_index]; 
  }

  
  void
  ActionTrackingWithCommand::addDesiredPose(vpHomogeneousMatrix & cdMo)
  {
    /// FIXME : it would be better to use iterators
    m_cdMoTab.push_back(cdMo);

  }

  void
  ActionTrackingWithCommand::clearDesiredPose()
  {
    
    m_cdMoTab.clear();
  }

  bool
  ActionTrackingWithCommand::nextDesiredPose()
  {
    m_index++;
    if (m_index >= m_cdMoTab.size())
      {
	m_index--;
	return false;
      }
    else
      return true;
       
  }

  
  bool
  ActionTrackingWithCommand::Initialize()
  {
    ODEBUG3("init");
    //m_LLVS->StartProcess(computeLawProcess_name.c_str());
    //ODEBUG3(computeLawProcess_name<<" started");
    // FIXME : always true
    return   m_trackerClient->Initialize();
  }

  bool
  ActionTrackingWithCommand::ExecuteAction()
  {
    //if the control law converges nextPose
    //if(movementFinished())
    //{
    //if(!nextDesiredPose())
    //  {
    //    m_LLVS->StopProcess(computeLawProcess_name.c_str());
    //    return false;
    //  }
    //}

    ODEBUG3("Execute Action");
    m_trackerClient->ExecuteAction();
   

    return true;
  }

  void
  ActionTrackingWithCommand::CleanUp()
  {
    //ActionTracking::CleanUp();
    m_LLVS->StopProcess(computeLawProcess_name.c_str());
    m_trackerClient->CleanUp();
    clearDesiredPose();
  }

  bool
  ActionTrackingWithCommand::movementFinished() const
  {

    vpHomogeneousMatrix cdMo = desiredPose()  ;
    vpHomogeneousMatrix cMo  = pose();
    vpHomogeneousMatrix cdMc = cdMo * cMo.inverse();
    vpPoseVector error (cdMc);
    vpColVector errorTest(3);
    errorTest[0] = error[0];
    errorTest[1] = error[2];
    errorTest[2] = error[4];

    return errorTest.infinityNorm() < m_threshold;
  }


  void 
  ActionTrackingWithCommand::readParameters()
  {
    ActionTracking::readParameters
      (getCommandFileFromModelName(modelName()));
    m_trackerClient->readParameters();
  }
} // end of namespace trackingClient.
