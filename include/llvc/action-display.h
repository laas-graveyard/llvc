// -*- c++-mode -*-
/*! ----------------------------------------------------
 *  Copyright 2010, CNRS-AIST JRL
 *
 *  \brief  Base class for all the tracking algorithm client side
 *  when launched, the client overwrite the server parameters
 * client and server are always synchronized
 * ---------------------------------------------------- */

#ifndef DISPLAY_TRACKING_CLIENT_H_
# define DISPLAY_TRACKING_CLIENT_H
# include <boost/shared_ptr.hpp>

# include <visp/vpDisplayX.h>
# include <visp/vpImage.h>

# include <llvs/tools/ActionWithLLVSBase.h>

# include <LowLevelVisionSystem.hh>

# include <llvc/action-grab.h>

namespace trackingClient
{
  /// \brief Generic display class.
  ///
  /// This class implements generic behavior for all displayers
  /// independently from the underlying algorithm (i.e. ActionTracking class).
  class ActionDisplay : public LLVClient::ActionWithLLVSBase
  {
  public:
    /// \brief Create a display from a grabbing client.
    ///
    /// \param gc shared pointer to grabbing client
    ActionDisplay(boost::shared_ptr<ActionGrab> gc);
    ~ActionDisplay();

    void display();
    virtual void initClick();
    virtual bool Initialize();
    virtual bool ExecuteAction();
    virtual void CleanUp();
   private:
    /// Pointer to LLVS server.
    LowLevelVisionSystem_var m_LLVS;
    /// Grabber pointer.
    boost::shared_ptr<ActionGrab> m_actionGrabClient;
    /// Grabbed image copy.
    vpImage<unsigned char> m_image;
    /// ViSP display to show image.
    vpDisplayX m_display;
  };
} // end of namespace trackingClient

#endif  // DISPLAY_TRACKING_CLIENT_H_
