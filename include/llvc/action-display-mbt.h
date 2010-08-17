// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#ifndef ACTION_DISPLAY_MBT_H_
# define ACTION_DISPLAY_MBT_H_
# include <boost/shared_ptr.hpp>

# include <visp/vpMbtTracker.h>
# include <visp/vpHomogeneousMatrix.h>

# include <llvc/action-display.h>
# include <llvc/action-tracking-mbt.h>

namespace trackingClient
{
  /// \brief Display class for model-based tracker.
  ///
  /// When using LLVS, the tracking is done on the
  /// server side. However, a vpMbtTracker is maintained
  /// on the client side for the initialization process
  /// (when initializing from clicks) and for the display
  /// (to reproject the model in the image and display
  /// the moving edges).
  class ActionDisplayMbt : public ActionDisplay
  {
  public:
    /// \brief Create a display from a grabbing client.
    ///
    /// The display manages initializes and manages
    /// the associated trackingClient to avoid loop
    /// problems between the vpMbtTracker initialization
    /// and the tracking client initialization.
    ///
    /// \param gc shared pointer to grabbing client
    /// \param modelName model name in the database
    /// \param configurationName configuration to be loaded for the
    ///        wanted model
    /// \param color color used for model projection in image
    ActionDisplayMbt(boost::shared_ptr<ActionGrab> gc,
		     const std::string& modelName,
		     const std::string& configurationName,
		     vpColor color = vpColor::blue,
		     bool logData = false);
    virtual ~ActionDisplayMbt();

    virtual bool Initialize();
    virtual bool ExecuteAction();
    virtual void CleanUp();

    /// \brief Return the pose determined from user clicks.
    const vpHomogeneousMatrix& initialPose() const
    {
      return m_initialPose;
    }

    /// \brief Enable or disable moving edge display.
    ///
    /// If enabled, colored points will be drawn on the
    /// image representing points discarded by the different
    /// steps of the tracking algorithm
    ///
    /// See vpMbtTracker documentation for more information.
    ///
    /// \param isEnabled true to show the dots, false to not show them
    void enableMovingEdgeDisplay(bool isEnabled)
    {
      m_tracker.setDisplayMovingEdges (isEnabled);
    }

    /// \brief Retrieve the tracking client.
    ///
    /// \return Shared pointer to the tracking client.
    boost::shared_ptr<ActionTrackingMbt> trackingClient()
    {
      return m_trackingClient;
    }

    /// \brief Pretty printing of the class on a stream
    ///
    /// \param stream stream used to display the instance informations
    /// \return stream
    virtual std::ostream& print (std::ostream& stream) const;

    /// \brief Write current debug information on disk.
    void logData() const;

  private:
    /// Tracking client.
    boost::shared_ptr<ActionTrackingMbt> m_trackingClient;
    /// Local tracker for initialization and display \bf only.
    vpMbtTracker m_tracker;
    /// Initial pose determined from user clicks.
    vpHomogeneousMatrix m_initialPose;
    /// Color used for model project on the image.
    vpColor m_color;
    /// Should this class writes its data on disk?
    bool m_logData;
  };
} // end of namespace trackingClient

#endif  // DISPLAY_TRACKING_CLIENT_H_
