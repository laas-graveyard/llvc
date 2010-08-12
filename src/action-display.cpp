// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

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

  std::ostream&
  ActionDisplay::print (std::ostream& stream) const
  {
    stream << "ActionDisplay:";
    return stream;
  }


  std::ostream& operator <<(std::ostream& stream,
			    const ActionDisplay& actionDisplay)
  {
    actionDisplay.print (stream);
    return stream;
  }
} // end of namespace trackingClient
