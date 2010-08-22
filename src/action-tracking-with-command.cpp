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
  static const std::string computeLawProcess_name = "ComputeControlLawProcess";

  ActionTrackingWithCommand::ActionTrackingWithCommand
  (boost::shared_ptr<ActionTracking> trackerClient)
    : ActionTracking(computeLawProcess_name, trackerClient->grabClient()),
      m_trackerClient(trackerClient)
  {
  }

  ActionTrackingWithCommand::~ActionTrackingWithCommand()
  { 
  }

  std::ostream&
  ActionTrackingWithCommand::print (std::ostream& stream) const
  {
    stream << "ActionTrackingWithCommand:";
    return stream;
  }

  bool
  ActionTrackingWithCommand::Initialize()
  {
    ActionTracking::Initialize();
    m_trackerClient->Initialize();
    return true;
  }

  bool
  ActionTrackingWithCommand::ExecuteAction()
  {
    //ActionTracking::ExecuteAction();
    m_trackerClient->ExecuteAction();
    return true;
  }

  void
  ActionTrackingWithCommand::CleanUp()
  {
    //ActionTracking::CleanUp();
    m_trackerClient->CleanUp();
  }

  bool
  ActionTrackingWithCommand::movementFinished() const
  {
    static const double threshold = 0.1;

    vpHomogeneousMatrix cdMc = m_cdMo * m_cMo.inverse();
    vpPoseVector error (cdMc);

    vpColVector errorTest(3);
    errorTest[0] = error[0];
    errorTest[1] = error[2];
    errorTest[2] = error[4];

    return errorTest.infinityNorm() < threshold;
  }
} // end of namespace trackingClient.
