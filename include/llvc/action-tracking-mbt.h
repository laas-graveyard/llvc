// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#ifndef ACTION_TRACKING_MBT_H_
# define ACTION_TRACKING_MBT_H_
# include <iosfwd>
# include <string>
# include <boost/shared_ptr.hpp>
# include <visp/vpMbtTracker.h>
# include <visp/vpHomogeneousMatrix.h>
# include <visp/vpFeaturePoint.h>
# include "llvc/action-tracking.h"

namespace trackingClient
{
  /// \brief Compute the ``.init'' file path from the model name.
  std::string getInitFileFromModelName (const std::string& modelName);

  /// \brief Compute the configuration file path from the model name
  /// and the configuration name.
  std::string getConfigurationFileFromModelName (const std::string& modelName,
						 const std::string& configurationName);

  /// \brief Compute the VRML path from the model name.
  std::string getModelFileFromModelName (const std::string& modelName);


  void convertViSPHomogeneousMatrixToCorba
  (const vpHomogeneousMatrix& visp,
   ModelTrackerInterface::HomogeneousMatrix_var& corba);

  void convertCorbaHomogeneousMatrixToVisp
  (const ModelTrackerInterface::HomogeneousMatrix_var& corba,
   vpHomogeneousMatrix& visp);

  void convertViSPHomogeneousMatrixToCorba
  (const vpHomogeneousMatrix& visp,
   ModelTrackerInterface::HomogeneousMatrix& corba);

  void convertCorbaHomogeneousMatrixToVisp
  (const ModelTrackerInterface::HomogeneousMatrix& corba,
   vpHomogeneousMatrix& visp);


  void convertCogToVisp
    (double CoG[2],
     vpFeaturePoint &vpfp);

  /// \brief Client for model-based tracker on LLVS.
  ///
  /// This client manages the process start/stop and the
  /// parameters synchronization and initialization.
  /// It also exposes the current cMo to the outside.
  class ActionTrackingMbt : public ActionTracking
  {
  public:
    typedef vpImage<unsigned char> image_t;

    /// \brief Construct the class using its server process name.
    ///
    /// \param processName process name on \bf server side
    ActionTrackingMbt(const vpHomogeneousMatrix& cMoIn,
		      boost::shared_ptr<ActionGrab> actionGrab,
		      const std::string& modelName,
		      const std::string& configurationName);
    virtual ~ActionTrackingMbt();
    virtual std::ostream& print (std::ostream& stream) const;

    virtual bool Initialize();
    virtual bool ExecuteAction();
    virtual void CleanUp();

    /// \brief Current pose retrieved from the server.
    const vpHomogeneousMatrix& pose() const
    {
      return m_cMo;
    }

    /// \brief Current pose retrieved from the server.
    const image_t& image() const
    {
      return m_image;
    }

    /// \brief Current timestamp retrieved from the server.
    const timestamp_t& timestamp() const
    {
      return m_timestamp;
    }

    /// \brief Returns vpFeaturePoint (CoG of the object to be tracked).
    const vpFeaturePoint& getCoG() const
    {
      return m_CoG;
    }
    /// \brief Import readParameters from mother class.
    using ActionTracking::readParameters;
    /// \brief Read configuration file and initialize parameters.
    void readParameters();

    /// \brief Model name in the database.
    const std::string& modelName() const
    {
      return m_modelName;
    }

    /// \brief Configuration profile for the wanted model.
    const std::string& configurationName() const
    {
      return m_configurationName;
    }

    void retrieveBufferData ();

  private:
    /// Current pose retrieved from the server.
    vpHomogeneousMatrix m_cMo;
    /// Image retrieved from debugInfoData.
    image_t m_image;

    //FIXME: search what it means.
    /// Timestamps.
    timestamp_t m_timestamp;
    
    /// CoG
    vpFeaturePoint m_CoG;
    /// Model name in the database.
    const std::string m_modelName;
    /// Configuration profile for the wanted model.
    const std::string m_configurationName;
  };

} // end of namespace trackingClient.

#endif // ! ACTION_TRACKING_MBT_H_
