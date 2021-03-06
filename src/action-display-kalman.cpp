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

#include "llvc/action-display-kalman.h"
#include "llvc/tools/indent.hh"

// LLVC_LOGGINGDIR is used to know where to log debug information.
#ifndef LLVC_LOGGINGDIR
# error "LLVC_LOGGINGDIR should be defined."
#endif //! LLVC_LOGGINGDIR

namespace trackingClient
{
  /// \brief Logging directory.
  static const std::string loggingDir = LLVC_LOGGINGDIR;

  ActionDisplayKalman::ActionDisplayKalman(boost::shared_ptr<ActionGrab> gc,
				     const std::string& modelName,
				     const std::string& configurationName,
				     vpColor color,
				     bool logData)
    : ActionDisplay(gc),
      m_trackingClient (),
      m_tracker(),
      m_initialPose(),
      m_color(color),
      m_logData(logData)
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
      boost::shared_ptr<ActionTrackingKalman>
      (new ActionTrackingKalman
       (m_initialPose,
	gc,
	modelName,
	configurationName));
  }

  ActionDisplayKalman::~ActionDisplayKalman()
  {
  }

  bool ActionDisplayKalman::Initialize()
  {
    m_trackingClient->Initialize();
    return ActionDisplay::Initialize();

  }

  bool ActionDisplayKalman::ExecuteAction()
  {

    m_trackingClient->ExecuteAction();
    m_image = m_trackingClient->image();

    //FIXME: send patch to enhance ViSP constness.
    // I.e. these copies would not be required if the constness
    // was handled correctly by vpKalmanTracker.
    vpCameraParameters cam = m_actionGrabClient->camera();
    vpHomogeneousMatrix cMo = m_trackingClient->pose();

    //FIXME: we do *not* grab using the grabbing
    //client anymore as ActionTrackingKalman::getBufferData
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

  void ActionDisplayKalman::CleanUp()
  {
    m_trackingClient->CleanUp();
    ActionDisplay::CleanUp();
  }

  std::ostream&
  ActionDisplayKalman::print (std::ostream& stream) const
  {
    ActionDisplay::print (stream);
    stream << iendl
	   << "ActionDisplayKalman:" << incindent << iendl
	   << "tracking color: " << m_color << iendl
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
  ActionDisplayKalman::logData() const
  {
    ODEBUG("in  ActionDisplayKalman::logData() ");
    static std::ofstream file(makeLogFilename("llvc-mbtkalman.log").c_str());
    static unsigned index = 0;

    if (!file)
      throw "failed to log data";

    if (!index)
      {
	file
	  << "# index (1) | timestamp.first (2) |  timestamp.second (3) | "
	  << " translation (4-6) | rx, ry, rz (7-9)" << std::endl;
      }

    const ActionTrackingKalman::image_t& image = m_trackingClient->image();
    const vpHomogeneousMatrix& cMo = m_trackingClient->pose();
    const ActionTrackingKalman::timestamp_t& timestamp = m_trackingClient->timestamp();

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

    boost::format fmtColor("Ires-mbtkalman-%04d.ppm");
    fmtColor % index;

    boost::format fmt("I-%04d.pgm");
    fmt % index;

    vpImageIo::writePPM(colorImage, makeLogFilename(fmtColor.str()));
    vpImageIo::writePGM(image, makeLogFilename(fmt.str()));
  }
} // end of namespace trackingClient
