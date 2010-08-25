// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#ifndef ACTION_DISPLAY_WITH_COMMAND_H_
# define ACTION_DISPLAY_WITH_COMMAND_H_
# include <boost/shared_ptr.hpp>

# include <visp/vpMbtTracker.h>
# include <visp/vpHomogeneousMatrix.h>

# include "llvc/action-display.h"
# include "llvc/action-tracking-with-command.h"

namespace trackingClient
{
 class ActionDisplayWithCommand : public ActionDisplay
  {
  public:
    /// FIXME : the types are defined in all the class
    /// there is may be a mean to factorise ?
    typedef std::vector<vpHomogeneousMatrix> homogeneousMatrix_t;
    ///
    /// FixMe:  Action dislay command only for nmbt tracker.
    ///
    ActionDisplayWithCommand
      (boost::shared_ptr<ActionGrab> gc,
       const std::string& modelName,
       const std::string& configurationName,
       unsigned desPoseNb = 1,
       vpColor color = vpColor::blue,
       bool logData = false,
       std::string  afileNameOfPoses=std::string(""));
    virtual ~ActionDisplayWithCommand();
    virtual bool Initialize();
    virtual bool ExecuteAction();
    virtual void CleanUp();

    /*! \name Methods to get a set of poses
      @{
    */

    /*! \brief  Read a file of poses. 
      \param return false if m_fileNameOfPoses is empty
      or if the filename cannot be open.
     */
    bool readFileOfPoses();

    /*! \brief Getting the poses through text user interface.
     Optionnally save the data for further usage.*/
    void userInterfaceInitPoses();

    /*! @} */
    /// \brief Return the pose determined from user clicks.
    const vpHomogeneousMatrix& initialPose() const
    {
      return m_initialPose;
    }
    void enableMovingEdgeDisplay(bool isEnabled)
    {
      m_tracker.setDisplayMovingEdges (isEnabled);
    }
    boost::shared_ptr<ActionTrackingWithCommand> trackingClient()
    {
      return m_trackingClient;
    }

    virtual std::ostream& print (std::ostream& stream) const;
    /// this function ask the user to click to init a pose
    bool clickToInitPose(vpHomogeneousMatrix & cMo);
    /// This function display images untill the user clicks
    /// it is usefull to position the robot
    void waitForUserClick(std::string t="");
    /// This function uses user clicks to init the desired pose
    void clickToInitDesiredPose(std::vector<vpHomogeneousMatrix>& desiredPoseList);
    /// \brief Write current debug information on disk.
    void logData() const;
    /// \brief read the config parameters
    void readParameters(); 
  
  private:
   
    /// Command client.
    boost::shared_ptr<ActionTrackingWithCommand> m_trackingClient;
    /// Tracking client.
    boost::shared_ptr<ActionTracking> m_actionTracking;
    /// Local tracker for initialization and display \bf only.
    vpMbtTracker m_tracker;
    /// Initial pose determined from user clicks.
    vpHomogeneousMatrix m_initialPose;
    // list of desired pose
    homogeneousMatrix_t  m_desiredPoseList;
    /// Nb of desired Pose to reach, default is 1
    unsigned m_desPoseNb; 
    /// Color used for model project on the image, default is blue
    vpColor m_color;
    /// Should this class writes its data on disk?, default is no
    bool m_logData;
    /// Name of the CAD model
    std::string  m_modelName;
    /// Name of the configuration file 
    std::string  m_configurationName;

    /// Name of the file storing the set of poses.
    std::string m_fileNameOfPoses;
    
   
  };
} // end of namespace trackingClient

#endif  // DISPLAY_TRACKING_CLIENT_H_
