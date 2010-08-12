/*! ----------------------------------------------------
 *  Copyright 2010, CNRS-AIST JRL
 *
 * ---------------------------------------------------- */

#include "llvc/action-display.h"

namespace trackingClient
{
  ActionDisplay::ActionDisplay
  (boost::shared_ptr<ActionGrab> gc)
    : LLVClient::ActionWithLLVSBase(),
      m_LLVS(GetServicePort<LowLevelVisionSystem,
	     LowLevelVisionSystem_var>()),
      m_actionGrabClient(gc),
      m_image(m_actionGrabClient->image()),
      m_display(m_image,10,10,"DisplayTracking")

  {
  }

  ActionDisplay::~ActionDisplay()
  {}

  void
  ActionDisplay::display()
  {

    vpDisplay::display(m_image);
    vpDisplay::flush(m_image);

  }

  void ActionDisplay::initClick()
  {
  }

  bool
  ActionDisplay::Initialize()
  {
    return true;
  }

  bool
  ActionDisplay::ExecuteAction()
  {
    m_image=m_actionGrabClient->image();
    display();
    return true;
  }

  void
  ActionDisplay::CleanUp()
  {
    std::cout << "USER >> Click to kill ActionDisplay" << std::endl;
    vpDisplay::getClick(m_image);
  }

} // end of namespace trackingClient
