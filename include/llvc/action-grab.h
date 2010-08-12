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
  /// \brief Grab an image from LLVS and expose it publicly.
  ///
  /// This class holds the responsibility of starting
  /// processes required to grab an image.
  ///
  /// Please note that this class will automatically
  /// stop the processes when destructed. By consequence,
  /// do not create more than one instance of this class
  /// to avoid premature stopping of the grabbing vision
  /// processes on server-side.
  class ActionGrab : public LLVClient::ActionWithLLVSBase
  {
  public:
    /// Alias to ViSP image type.
    typedef vpImage<unsigned char> image_t;

    explicit ActionGrab();
    virtual ~ActionGrab();
    virtual std::ostream& print (std::ostream& stream) const;

    virtual bool Initialize();
    virtual bool ExecuteAction();
    virtual void CleanUp();

    const image_t& image() const
    {
      return m_image;
    }

  private:
    /// Pointer to the server LLVS.
    LowLevelVisionSystem_var m_LLVS;
    /// Grabbed image.
    image_t m_image;
    /// Semantic camera id.
    long m_cameraID;
    /// Image format.
    std::string m_format;
  };

  std::ostream& operator <<(std::ostream& stream,
			    const ActionGrab& actionTrackingClient);
}
#endif
