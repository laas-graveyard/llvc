/*! ----------------------------------------------------
 *  Copyright 2010, CNRS-AIST JRL
 * ---------------------------------------------------- */

#ifndef ACTION_TRACKING_CLIENT_H_
# define ACTION_TRACKING_CLIENT_H_

# include <iosfwd>
# include <string>
# include <vector>

# include <visp/vpHomogeneousMatrix.h>
# include <visp/vpImage.h>

# include <LowLevelVisionSystem.hh>

# include <llvs/tools/CORBAClient.h>
# include <llvs/tools/ActionWithLLVSBase.h>

namespace trackingClient
{

  /// \brief Base class for all the tracking algorithm client side
  /// WARNING when launched, the client overwrite the server parameters
  /// client and server are always synchronized.
  class ActionTrackingClient : public LLVClient::ActionWithLLVSBase
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
    /// If necessary, please rebuild an ActionTrackingClient.
    ///
    /// \param processName process name on \bf server side
    ActionTrackingClient(const std::string& processName);
    virtual ~ActionTrackingClient();

    //void setParamList(const paramList_t& p){m_paramList=p;}

    const paramList_t& getParamList()
    {
      return m_paramList;
    }

    const image_t& getImage()
    {
      return m_image;
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

    const image_t& image() const
    {
      return m_image;
    }

    const std::string m_serverProcessName;

 protected:
    image_t& image()
    {
      return m_image;
    }

    virtual void grabImageServer();
    virtual void track() = 0;



  private:
    /*! Get the data from the bufer on LLVS. */
    // int getServerLog( ModelTrackerInterface_var aMTI,
    //		      vpImage<unsigned char> &aIvisp,
    //		      vpHomogeneousMatrix &acMo,
    //	      std::pair <long,long> &TimeStamp);

    /*! \brief Pointer to the server LLVS. */
    LowLevelVisionSystem_var m_LLVS;

    // list the parameters that have been set
    paramList_t m_paramList;

    image_t m_image;

    long m_cameraID;
    std::string m_format;
  };

  std::ostream& operator <<(std::ostream& stream,
			    const ActionTrackingClient::paramList_t& paramList);
  std::ostream& operator <<(std::ostream& stream,
			    const ActionTrackingClient& actionTrackingClient);
}
#endif
