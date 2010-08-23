// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#ifndef ACTION_TRACKING_WITH_COMMAND_H_
# define ACTION_TRACKING_WITH_COMMAND_H_
# include <iosfwd>
# include <string>
# include <vector>

# include <visp/vpHomogeneousMatrix.h>

# include <boost/shared_ptr.hpp>
# include <llvc/action-tracking-mbt.h>

namespace trackingClient
{
  /// \brief Client for command law computation.
  /// This class select the desired position cdMo 
  /// among a list of cdMo and send it to the server
  /// it also start and stop the command process
  ///  
  /// 
  /// 
  class ActionTrackingWithCommand : public ActionTracking
  {
  public:
    typedef std::vector<vpHomogeneousMatrix> homogeneousMatrix_t;
    typedef vpImage<unsigned char> image_t;
    typedef std::pair<long int, long int> timestamp_t;
 
    /// \brief Construct the class using its server process name.
    ///
    ActionTrackingWithCommand
      (boost::shared_ptr<ActionTracking> trackerClient,
       homogeneousMatrix_t& desiredPoseList,
       double threshold = 1e-2,
       unsigned currentIndex = 0 );
    virtual ~ActionTrackingWithCommand();

    virtual std::ostream& print (std::ostream& stream) const;
   
    virtual bool Initialize();
    virtual bool ExecuteAction();
    virtual void CleanUp();
 
    void addDesiredPose(vpHomogeneousMatrix & cdMo);
    void clearDesiredPose();
    
    const timestamp_t& timestamp()const
    {
      return m_trackerClient->timestamp();
    }

    const image_t& image()const
    {
      return  m_trackerClient->image();
    }
 
    const double& threshold()
    {
      return m_threshold;
    }
    const std::vector<vpHomogeneousMatrix>& poseList()const
    {
      return m_cdMoTab;
    }
    // return current tracked pose
    const vpHomogeneousMatrix& pose() const
    {
      return m_trackerClient->pose();
    }
    /// \brief Return true if the desired cMo has been reached.
    bool movementFinished() const;

  private:
    const vpHomogeneousMatrix& desiredPose() const;

  private:
    /// \brief Shared pointer on the tracking client.
    boost::shared_ptr<ActionTracking> m_trackerClient;
    // desired position
    homogeneousMatrix_t& m_cdMoTab ;
    // threshold to stop the control
    double m_threshold;
    // current index
    unsigned m_index;
     
    

  
  };

} // end of namespace trackingClient.

#endif // ! ACTION_TRACKING_WITH_COMMAND_H_
