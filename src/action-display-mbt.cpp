// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include "llvc/action-display-mbt.h"
#include "llvc/tools/indent.hh"

namespace trackingClient
{
  ActionDisplayMbt::ActionDisplayMbt(boost::shared_ptr<ActionGrab> gc,
				     const std::string& modelName,
				     const std::string& configurationName,
				     vpColor color)
    : ActionDisplay(gc),
      m_trackingClient (),
      m_tracker(),
      m_initialPose(),
      m_color(color)
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
    m_tracker.loadModel(getModelFileFromModelName (modelName).c_str());
    m_tracker.initClick(m_image, getInitFileFromModelName(modelName).c_str());
    m_tracker.getPose(m_initialPose);
    m_trackingClient =
      boost::shared_ptr<ActionTrackingMbt>
      (new ActionTrackingMbt
       (m_initialPose,
	gc,
	modelName,
	configurationName));
  }

  ActionDisplayMbt::~ActionDisplayMbt()
  {
  }

  bool ActionDisplayMbt::Initialize()
  {
    m_trackingClient->Initialize();
    return ActionDisplay::Initialize();

  }

  bool ActionDisplayMbt::ExecuteAction()
  {
    m_trackingClient->ExecuteAction();

    //FIXME: send patch to enhance ViSP constness.
    // I.e. these copies would not be required if the constness
    // was handled correctly by vpMbtTracker.
    vpCameraParameters cam = m_actionGrabClient->camera();
    vpHomogeneousMatrix cMo = m_trackingClient->pose();

    if (!ActionDisplay::ExecuteAction())
      return false;
    m_tracker.display (m_image, cMo, cam, m_color);
    vpDisplay::flush(m_image);
    return true;
  }

  void ActionDisplayMbt::CleanUp()
  {
    m_trackingClient->CleanUp();
    ActionDisplay::CleanUp();
  }

  std::ostream&
  ActionDisplayMbt::print (std::ostream& stream) const
  {
    ActionDisplay::print (stream);
    stream << iendl
	   << "ActionDisplayMbt:" << incindent << iendl
	   << "tracking color: " << m_color << iendl
	   << "tracking client:" << incindent << iendl;

    if (m_trackingClient)
      stream << *m_trackingClient;
    else
      stream << "no tracking client";

    stream << decindent << decindent;
    return stream;
  }
} // end of namespace trackingClient
