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
#include "llvc/tools/indent.hh"
#include "llvc/tools/visp-io.hh"

namespace trackingClient
{
  static const std::string defaultPath = "./data/model/";
  static const std::string trackingProcess_name="nmbtTrackingProcess";
  // Without the extension as ViSP wants it like that.
  std::string getInitFileFromModelName (const std::string& modelName)
  {
    std::string res(defaultPath);
    res += modelName + "/" + modelName;
    return res;
  }

  std::string getConfigurationFileFromModelName (const std::string& modelName,
						 const std::string& configurationName)
  {
    std::string res(defaultPath);
    res += modelName + "/" + modelName + "-" + configurationName + ".conf";
    return res;
  }

  std::string getModelFileFromModelName (const std::string& modelName)
  {
    std::string res(defaultPath);
    res += modelName + "/" + modelName + ".wrl";
    return res;
  }




  void convertViSPHomogeneousMatrixToCorba
  (const vpHomogeneousMatrix& visp,
   ModelTrackerInterface::HomogeneousMatrix_var& corba)
  {
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
	corba->cMo[i][j] = visp[i][j];
  }

  void convertCorbaHomogeneousMatrixToVisp
  (const ModelTrackerInterface::HomogeneousMatrix_var& corba,
   vpHomogeneousMatrix& visp)
  {
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
	visp[i][j] = corba->cMo[i][j];
  }

  void convertViSPHomogeneousMatrixToCorba
  (const vpHomogeneousMatrix& visp,
   ModelTrackerInterface::HomogeneousMatrix& corba)
  {
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
	corba.cMo[i][j] = visp[i][j];
  }

  void convertCorbaHomogeneousMatrixToVisp
  (const ModelTrackerInterface::HomogeneousMatrix& corba,
   vpHomogeneousMatrix& visp)
  {
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
	visp[i][j] = corba.cMo[i][j];
  }

  ActionTrackingMbt::ActionTrackingMbt
  (const vpHomogeneousMatrix& cMo,
   boost::shared_ptr<ActionGrab> actionGrab,
   const std::string& modelName,
   const std::string& configurationName)
    : ActionTracking(trackingProcess_name, actionGrab),
      m_cMo(cMo),
      m_image(actionGrab->image().getHeight(),actionGrab->image().getWidth()),
      m_timestamp(),
      m_modelName(modelName),
      m_configurationName(configurationName)
  {
    ModelTrackerInterface::HomogeneousMatrix_var cMoCorba;
    convertViSPHomogeneousMatrixToCorba(cMo, cMoCorba);

    ModelTrackerInterface_var serverTracker;
    serverTracker = m_LLVS->getModelTracker();
    serverTracker->SetcMo(cMoCorba);

    setTrackingParameters
      ("PATH_MODEL", getModelFileFromModelName (m_modelName));
    readParameters();
  }

  ActionTrackingMbt::~ActionTrackingMbt()
  {


  }


  std::ostream&
  ActionTrackingMbt::print (std::ostream& stream) const
  {
    ActionTracking::print(stream);
    stream << iendl
	   << "ActionTrackingMbt:" << incindent << iendl
	   << "model name: " << m_modelName << iendl
	   << "configuration name: " << m_configurationName << iendl
	   << "cMo: " << incindent << iendl;
    display (stream, m_cMo);
    stream << decindent << decindent;
    return stream;
  }

  bool ActionTrackingMbt::Initialize()
  {

    return ActionTracking::Initialize();
  }

  bool ActionTrackingMbt::ExecuteAction()
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

  void ActionTrackingMbt::CleanUp()
  {}

  void ActionTrackingMbt::readParameters()
  {
    readParameters
      (getConfigurationFileFromModelName(m_modelName, m_configurationName));
  }

  void ActionTrackingMbt::retrieveBufferData()
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
