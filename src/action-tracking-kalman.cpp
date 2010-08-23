// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.
#include "llvc/action-tracking-mbt.h"
#include "llvc/action-tracking-kalman.h"
#include "llvc/tools/indent.hh"
#include "llvc/tools/visp-io.hh"

namespace trackingClient
{
  static const std::string defaultPath = "./data/model/";
  static const std::string trackingKalmanProcess_name = "KalmanOnNMBTProcess";

  ActionTrackingKalman::ActionTrackingKalman
  (const vpHomogeneousMatrix& cMo,
   boost::shared_ptr<ActionGrab> actionGrab,
   const std::string& modelName,
   const std::string& configurationName)
    : ActionTracking("trackingKalmanProcess_name", actionGrab),
      m_cMo(cMo),
      m_image(actionGrab->image().getHeight(),actionGrab->image().getWidth()),
      m_timestamp(),
      m_modelName(modelName),
      m_configurationName(configurationName)
  {
    
    // if()  
 
    ModelTrackerInterface::HomogeneousMatrix_var cMoCorba;
    convertViSPHomogeneousMatrixToCorba(cMo, cMoCorba);

    ModelTrackerInterface_var serverTracker;
    serverTracker = m_LLVS->getModelTracker();
    serverTracker->SetcMo(cMoCorba);

    setTrackingParameters
      ("PATH_MODEL", getModelFileFromModelName (m_modelName));
    readParameters();
  }

  ActionTrackingKalman::~ActionTrackingKalman()
  {
    

  }


  std::ostream&
  ActionTrackingKalman::print (std::ostream& stream) const
  {
    ActionTracking::print(stream);
    stream << iendl
	   << "ActionTrackingKalman:" << incindent << iendl
	   << "model name: " << m_modelName << iendl
	   << "configuration name: " << m_configurationName << iendl
	   << "cMo: " << incindent << iendl;
    display (stream, m_cMo);
    stream << decindent << decindent;
    return stream;
  }

  bool ActionTrackingKalman::Initialize()
  {

    return ActionTracking::Initialize();
  }

  bool ActionTrackingKalman::ExecuteAction()
  {
    int Verbose = 0;
    m_LLVS->TriggerSynchro();

    retrieveBufferData();

    if (Verbose > 2)
      {
	std::cout << "cMo retrieved from server by retrieveBuferData:"
		  << incindent << iendl;
	display(std::cout, m_cMo);
	std::cout << decindent;
      }

    return true;
  }

  void ActionTrackingKalman::CleanUp()
  {}

  void ActionTrackingKalman::readParameters()
  {
    readParameters
      (getConfigurationFileFromModelName(m_modelName, m_configurationName));
  }

  void ActionTrackingKalman::retrieveBufferData()
  {
    ModelTrackerInterface_var MTI = m_LLVS->getModelTracker();
    ModelTrackerInterface::DebugInfoObject_var debugObject;

    MTI->GetDebugInfoObject(debugObject);

    convertCorbaImageToVispImage
      (debugObject->anImgData, m_image);

    convertCorbaHomogeneousMatrixToVisp
      (debugObject->aData, m_cMo);

    m_timestamp.first = debugObject->anImgData.longData[0];
    m_timestamp.second = debugObject->anImgData.longData[1];
  }

} // end of namespace trackingClient.
