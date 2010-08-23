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
  /// \brief Installation prefix.
  static const std::string prefix = CMAKE_INSTALL_PREFIX;
  /// \brief Logging directory.
  static const std::string loggingDir = prefix + "/var/log";
  
  static const std::string computeLawProcess_name = "ComputeControlLawProcess";

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
  }

  ActionTrackingWithCommand::~ActionTrackingWithCommand()
  { 
  }

  std::ostream&
  ActionTrackingWithCommand::print (std::ostream& stream) const
  {
    stream << "ActionTrackingWithCommand:"<<iendl;
    stream << iendl
	   << "cMo: " << incindent << iendl;
    display (stream, pose());
    stream << "cdMo: " << incindent << iendl;
    display (stream, desiredPose());
    stream << "velocity :" <<incindent << iendl;
    stream << decindent << decindent;
    return stream;
  }
  
  const vpHomogeneousMatrix&   
  ActionTrackingWithCommand::desiredPose() const
  {
    if (m_index<0 | m_index > m_cdMoTab.size())
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
  ActionTrackingWithCommand::Initialize()
  {
    ActionTracking::Initialize();
    m_trackerClient->Initialize();
    m_LLVS->StartProcess("ComputeControlLawProcess");
    ODEBUG3("ComputeControlLawProcess started");
    return true;
  }

  bool
  ActionTrackingWithCommand::ExecuteAction()
  {
    //if the control law converges stop process
    if(movementFinished())
      {
	m_LLVS->StopProcess("ComputeControlLawProcess");
	return false;
      }
    m_trackerClient->ExecuteAction();
    return true;
  }

  void
  ActionTrackingWithCommand::CleanUp()
  {
    //ActionTracking::CleanUp();
    m_LLVS->StopProcess("ComputeControlLawProcess");
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
} // end of namespace trackingClient.
