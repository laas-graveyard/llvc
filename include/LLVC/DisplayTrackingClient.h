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

# include <llvs/tools/ActionWithLLVSBase.h>


# include <LLVC/ActionGrabClient.h>
# include <visp/vpDisplayX.h>

namespace trackingClient
{
  /*! \brief Class to get connected to a remote corba client providing
    bipedal walking capabilities. */
  class DisplayTrackingClient : public LLVClient::ActionWithLLVSBase
  {
  public:
    DisplayTrackingClient(boost::shared_ptr<ActionGrabClient> gc);
    ~DisplayTrackingClient();
    void display();
    virtual void initClick();
    virtual bool Initialize();
    virtual bool ExecuteAction();
    virtual void CleanUp();
   private:


    vpDisplayX m_display;
    vpImage<unsigned char> m_image;
    LowLevelVisionSystem_var m_LLVS;
    boost::shared_ptr<ActionGrabClient> m_actionGrabClient;


  };
} // end of namespace trackingClient

#endif  // DISPLAY_TRACKING_CLIENT_H_
