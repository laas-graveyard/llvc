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

# include <boost/shared_ptr.hpp>
# include <llvc/action-tracking.h>

namespace trackingClient
{
  /// \brief Client for command law computation.
  ///
  class ActionTrackingWithCommand : public ActionTracking
  {
  public:
    /// \brief Construct the class using its server process name.
    ///
    ActionTrackingWithCommand
    (boost::shared_ptr<ActionTracking> trackerClient);

    virtual ~ActionTrackingWithCommand();
    virtual std::ostream& print (std::ostream& stream) const;

    virtual bool Initialize();
    virtual bool ExecuteAction();
    virtual void CleanUp();

    /// \brief Return true if the desired cMo has been reached.
    bool movementFinished() const;
  private:
    /// \brief Shared pointer on the tracking client.
    boost::shared_ptr<ActionTracking> m_trackerClient;

    //
    vpHomogeneousMatrix m_cMo;
    vpHomogeneousMatrix m_cdMo;
  };

} // end of namespace trackingClient.

#endif // ! ACTION_TRACKING_WITH_COMMAND_H_
