/*! ----------------------------------------------------
 *  Copyright 2010, CNRS-AIST JRL
 *
 * ---------------------------------------------------- */

#include "LLVC/DisplayTrackingClient.h"

namespace trackingClient
{
  DisplayTrackingClient::DisplayTrackingClient
  (boost::shared_ptr<ActionGrabClient> gc)
    : LLVClient::ActionWithLLVSBase(),
      m_display(),
      m_image(),
      m_LLVS(GetServicePort<LowLevelVisionSystem,
	     LowLevelVisionSystem_var>()),
      m_actionGrabClient(gc)

  {

    m_image=m_actionGrabClient->image();
    m_display.init(m_image,10,10,"DisplayTracking");


  }

  DisplayTrackingClient::~DisplayTrackingClient()
  {}

  void
  DisplayTrackingClient::display()
  {

    vpDisplay::display(m_image);
    vpDisplay::flush(m_image);

  }

  void DisplayTrackingClient::initClick()
  {
  }

  bool
  DisplayTrackingClient::Initialize()
  {

    return true;
  }

  bool
  DisplayTrackingClient::ExecuteAction()
  {
    m_image=m_actionGrabClient->image();
    display();
    return true;
  }

  void
  DisplayTrackingClient::CleanUp()
  {
    std::cout << "USER >> Click to kill DisplayTrackingClient" << std::endl;

    vpDisplay::getClick(m_image);

  }

} // end of namespace trackingClient
