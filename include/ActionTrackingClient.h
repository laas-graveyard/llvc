/*! ----------------------------------------------------
 *  Copyright 2010, CNRS-AIST JRL
 * 
 *  \brief  Base class for all the tracking algorithm client side 
 *  when launched, the client overwrite the server parameters
 * client and server are always synchronized
 * ---------------------------------------------------- */

#ifndef ACTION_TRACKING_CLIENT_H_
#define ACTION_TRACKING_CLIENT_H_

#include <vector>

#include <visp/vpHomogeneousMatrix.h>

#include <LowLevelVisionSystem.hh>

#include <llvs/tools/CORBAClient.h>
#include <llvs/tools/ActionWithLLVSBase.h>

namespace trackingClient
{
    
  /*! \brief Class to get connected to a remote corba client providing 
    bipedal walking capabilities. */
  class ActionTrackingClient : public LLVClient::ActionWithLLVSBase
  {
  public:
    typedef std::vector< std::pair<std::string,std::string> > paramList_t;
    typedef vpImage<unsigned char> image_t;
    static ActionTrackingClient createFromClick ();
    static ActionTrackingClient createFromCMo (const vpHomogeneousMatrix& cMo);
    virtual ~ActionTrackingClient();
    virtual bool Initialize();
    virtual bool ExecuteAction();
    virtual void CleanUp();
    //void setParamList(const paramList_t& p){m_paramList=p;}
    const paramList_t& getParamList(){return m_paramList;}
    const image_t& getImage(){return m_image;};
    virtual std::ostream& print (std::ostream& stream);
    const std::string m_serverProcessName;
    
    /*set tracking parameters client side and server side*/   
    void setTrackingParameters(std::string paramName, std::string paramValue);
    virtual void impl_setTrackingParameters(std::string paramName, std::string paramValue)=0;

 protected:
    ActionTrackingClient(const vpHomogeneousMatrix& cMo);
    virtual void setImage(const image_t& image ){m_image=image;} 
   
   
      
    /*Need the user click to initialise*/
    virtual void initTrackingClick();
    virtual void initTrackingCMo();
    virtual void grabImageServer();
    virtual void track();

    
  
  private:
    /*! Get the data from the bufer on LLVS. */
    int getServerLog( ModelTrackerInterface_var aMTI,
		      vpImage<unsigned char> &aIvisp, 
		      vpHomogeneousMatrix &acMo,
		      std::pair <long,long> &TimeStamp);

    /*! \brief Pointer to the server LLVS. */
    LowLevelVisionSystem_var m_LLVS;

    // list the parameters that have been set
    paramList_t& m_paramList;

    image_t& m_image; 
  

  };

  std::ostream& operator <<(std::ostream& stream,
			    const ActionTrackingClient::paramList_t& paramList);
  std::ostream& operator <<(std::ostream& stream,
			    const ActionTrackingClient& actionTrackingClient)
    {
      return actionTrackingClient.print (stream);
    }
}
#endif  
