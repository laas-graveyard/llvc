/*! ----------------------------------------------------
 *  Copyright 2010, CNRS-AIST JRL
 *
 * ---------------------------------------------------- */

#include <iostream>

#include "LowLevelVisionSystem.hh"

#include "llvc/action-tracking.h"

namespace trackingClient
{
  ActionTracking::ActionTracking(const std::string& processName)
    : LLVClient::ActionWithLLVSBase(),
      m_serverProcessName(processName),
      m_LLVS(GetServicePort<LowLevelVisionSystem,
	     LowLevelVisionSystem_var>()),
      m_paramList()
  {
  }

  ActionTracking::~ActionTracking()
  {
  }

  std::ostream&
  ActionTracking::print (std::ostream& stream) const
  {
    stream << "ActionTracking:" << std::endl
	   << "\t process name: " << m_serverProcessName << std::endl;
    return stream;
  }

  void ActionTracking::setTrackingParameters(std::string paramName,
					     std::string paramValue)
  {
    m_LLVS->SetAProcessParameterAndValue(m_serverProcessName.c_str (),
					 paramName.c_str (),
					 paramValue.c_str ());
    impl_setTrackingParameters(paramName, paramValue);
  }

  std::ostream& operator <<(std::ostream& stream,
			    const ActionTracking& actionTrackingClient)
  {
    return actionTrackingClient.print (stream);
  }

} // end of namespace trackingClient
