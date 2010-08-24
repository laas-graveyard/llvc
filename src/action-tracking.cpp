// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include <cassert>

#include <algorithm>
#include <iostream>
#include <fstream>
#include "LowLevelVisionSystem.hh"

#include "llvc/action-tracking.h"
#include "llvc/tools/indent.hh"

namespace trackingClient
{
  ActionTracking::ActionTracking(const std::string& processName,
				 boost::shared_ptr<ActionGrab> actionGrab)
    : LLVClient::ActionWithLLVSBase(),
      m_serverProcessName(processName),
      m_LLVS(GetServicePort<LowLevelVisionSystem,
	     LowLevelVisionSystem_var>()),
      m_paramList(),
      m_grabClient (actionGrab)
  {
    assert (actionGrab);
    getServerParameters();

  }

  ActionTracking::~ActionTracking()
  {
    m_LLVS->StopProcess("CircularModelTrackerData");
    m_LLVS->StopProcess(m_serverProcessName.c_str());
  }

  bool
  ActionTracking::Initialize()
  {
    ODEBUG3("init action tracking " <<m_serverProcessName.c_str() );
    
    m_LLVS->StartProcess(m_serverProcessName.c_str());
    m_LLVS->StartProcess("CircularModelTrackerData");
    sleep(1);
    ODEBUG3("end of init action tracking");
    return true;
  }
  
 

  //FIXME: fill by default the paramList. As the server side
  //is only filled after a set.
  std::ostream&
  ActionTracking::print (std::ostream& stream) const
  {
    stream << "ActionTracking:" << incindent << iendl
	   << "process name: " << m_serverProcessName << iendl
	   << "parameters list (" << m_paramList.size () << "): "
	   << incindent << iendl
	   << m_paramList << decindent << decindent;
    return stream;
  }

  void ActionTracking::setTrackingParameters(std::string paramName,
					     std::string paramValue)
  {
    m_LLVS->SetAProcessParameterAndValue(m_serverProcessName.c_str (),
					 paramName.c_str (),
					 paramValue.c_str ());
  }


  ///
  /// \brief search predicate used for get server parameters methods
  ///
  namespace
  {
    struct searchPredicate
      : public std::unary_function<const std::pair<std::string, std::string>&, bool>
    {
      searchPredicate(const std::string& value)
	: value_ (value)
      {}

      bool operator()
      (const std::pair<std::string, std::string>& pair)
      {
	return pair.first == value_;
      }
    private:
      const std::string& value_;
    };
  } // end of anonymouse namespace.

  void ActionTracking::getServerParameters()
  {

    ODEBUG("In ActionTracking::getServerParameters()");
    HRP2LowLevelVisionParametersSeq_var paramNameSeq,paramValueSeq;

    m_LLVS->GetProcessParameters(m_serverProcessName.c_str(),
				 paramNameSeq,
				 paramValueSeq);
    ODEBUG("paramNameSeq->length()=" << paramNameSeq->length());
    for(unsigned i = 0 ; i < paramNameSeq->length(); ++i)
      {
	std::string paramName(paramNameSeq[i]);
	std::string paramValue(paramValueSeq[i]);
        ODEBUG("param server: "<< paramName <<"=" << paramValue);
	searchPredicate pred(paramValue);
	paramList_t::iterator it =
	  std::find_if (m_paramList.begin (), m_paramList.end (), pred);
	if (it == m_paramList.end ())
	  m_paramList.push_back(std::make_pair(paramName, paramValue));
	else
	  *it = std::make_pair(paramName, paramValue);
      }
  }

  ///
  /// read config file
  /// 1st line   :  number of params
  /// other lines:  paramName paramValue
   void ActionTracking::readParameters(const std::string& filename)
   {
     std::ifstream configFile(filename.c_str());
     if(!configFile)
       throw "File failed to open";
     else
       {
	
 	int paramNb = 0;
 	configFile >> paramNb;

  	for(int i=0;i < paramNb;++i)
	  {
	    std::string paramName,paramValue;
	    configFile >> paramName >> paramValue;
	    ODEBUG(m_serverProcessName.c_str()<<" : "<< paramName <<"=" << paramValue);
	    m_LLVS->SetAProcessParameterAndValue(m_serverProcessName.c_str(),
						 paramName.c_str(),
						 paramValue.c_str());
	  }
       }
     // update local parameter list for print
     getServerParameters();
   }

  std::ostream& operator <<(std::ostream& stream,
			    const ActionTracking& actionTrackingClient)
  {
    actionTrackingClient.print (stream);
    return stream;
  }

  std::ostream& operator <<(std::ostream& stream,
			    const ActionTracking::paramList_t& paramList)
  {
    if (paramList.empty())
      {
	stream << "no parameters";
	return stream;
      }
    ActionTracking::paramList_t::const_iterator it;
    for (it = paramList.begin(); it < paramList.end(); ++it)
      stream << it->first << ": " << it->second << iendl;
    return stream;
  }

} // end of namespace trackingClient
