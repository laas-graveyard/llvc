/*! ----------------------------------------------------
 *  Copyright 2010, CNRS-AIST JRL
 *
 * ---------------------------------------------------- */

#include "LLVC/DisplayTrackingClient.h"

namespace trackingClient
{
  DisplayTrackingClient::DisplayTrackingClient()
    : LLVClient::ActionWithLLVSBase(),
      m_display(),
      m_LLVS(GetServicePort<LowLevelVisionSystem,
	     LowLevelVisionSystem_var>()),
      m_actionTrackingClient()
  {}

  DisplayTrackingClient::~DisplayTrackingClient()
  {}

  void
  DisplayTrackingClient::display()
  {}

  bool
  DisplayTrackingClient::Initialize()
  {
    return true;
  }

  bool
  DisplayTrackingClient::ExecuteAction()
  {
    return true;
  }

  void
  DisplayTrackingClient::CleanUp()
  {}
} // end of namespace trackingClient
