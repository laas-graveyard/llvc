/*! ----------------------------------------------------
 *  Copyright 2010, CNRS-AIST JRL
 * 
 *  \brief  Base class for all the tracking algorithm client side 
 *  when launched, the client overwrite the server parameters
 * client and server are always synchronized
 * ---------------------------------------------------- */

#ifndef DISPLAY_TRACKING_CLIENT_H_
#define DISPLAY_TRACKING_CLIENT_H_

#include <llvs/tools/ActionTrackingClient.h>
#include <visp/vpDisplayX.h>

namespace trackingClient
{
    
  /*! \brief Class to get connected to a remote corba client providing 
    bipedal walking capabilities. */
  class DisplayTrackingClient
  {
  public:
    DisplayTrackingClient(const ActionTrackingClient& actionTC);
    void display();
  private:
    vpDisplayX display;
    const ActionTrackingClient& m_actionTrackingClient;
  };


}
#endif  
