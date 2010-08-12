/*! ----------------------------------------------------
 *  Copyright 2010, CNRS-AIST JRL
 * ---------------------------------------------------- */

#ifndef ACTION_TRACKING_CLIENT_H_
# define ACTION_TRACKING_CLIENT_H_

# include <iosfwd>
# include <string>
# include <vector>

# include <boost/shared_ptr.hpp>

# include <visp/vpHomogeneousMatrix.h>
# include <visp/vpImage.h>

# include <LowLevelVisionSystem.hh>

# include <llvs/tools/CORBAClient.h>
# include <llvs/tools/ActionWithLLVSBase.h>

# include <llvc/action-grab.h>

namespace trackingClient
{

  /// \brief Base class for all the tracking algorithm client side
  ///
  /// \warning When launched, the client overwrite the server parameters
  /// client and server are always synchronized.
  class ActionTracking : public LLVClient::ActionWithLLVSBase
  {
  public:
    typedef std::vector< std::pair<std::string,std::string> > paramList_t;
    typedef vpImage<unsigned char> image_t;

    /// \brief Construct the class using its server process name.
    ///
    /// At construction, camera parameters are retrieved from the
    /// server side to get image size.
    /// It also retrieves a CORBA connection through CORBAReference.
    ///
    /// \warning This class makes the assumption that the current
    /// camera and/or the camera size will never be changed at runtime.
    /// If necessary, please rebuild an ActionTracking.
    ///
    /// \param processName process name on \bf server side
    ActionTracking(const std::string& processName);
    virtual ~ActionTracking();

    const paramList_t& getParamList()
    {
      return m_paramList;
    }

    virtual std::ostream& print (std::ostream& stream) const;

    /// \brief Set tracking parameter on both client and server sides.
    ///
    /// This method forwards the change on server side automatically and
    /// calls ipl_setTrackingParameters to allow additional changes in
    /// the children classes.
    ///
    /// \param paramName parameter name
    /// \param paramValue parameter value
    void setTrackingParameters(std::string paramName, std::string paramValue);

    virtual void impl_setTrackingParameters(std::string paramName,
					    std::string paramValue) = 0;

    const std::string m_serverProcessName;

 protected:
    virtual void track() = 0;

  private:
    /// Pointer to LLVS server.
    LowLevelVisionSystem_var m_LLVS;
    /// Parameters list.
    paramList_t m_paramList;
    /// Pointer to grabbing client.
    boost::shared_ptr<ActionGrab> m_grabClient;
  };

  std::ostream& operator <<(std::ostream& stream,
			    const ActionTracking::paramList_t& paramList);
  std::ostream& operator <<(std::ostream& stream,
			    const ActionTracking& actionTrackingClient);
}
#endif
