// -*- c++-mode -*-
/*! ----------------------------------------------------
 *  Copyright 2010, CNRS-AIST JRL
 *
 * ---------------------------------------------------- */

#ifndef ACTION_GRAB_CLIENT_H_
# define ACTION_GRAB_CLIENT_H_

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
  class ActionGrabClient : public LLVClient::ActionWithLLVSBase
  {
  public:
    typedef vpImage<unsigned char> image_t;

    ActionGrabClient();
    virtual ~ActionGrabClient();
    virtual std::ostream& print (std::ostream& stream) const;

    virtual bool Initialize();
    virtual bool ExecuteAction();
    virtual void CleanUp();

    const image_t& image() const
    {
      return m_image;
    }

 protected:
//     image_t& image()
//     {
//       return m_image;
//     }

  private:
    /// Pointer to the server LLVS.
    LowLevelVisionSystem_var m_LLVS;

    image_t m_image;

    long m_cameraID;
    std::string m_format;
  };

  std::ostream& operator <<(std::ostream& stream,
			    const ActionGrabClient& actionTrackingClient);
}
#endif
