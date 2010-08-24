// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#ifndef ACTION_GRAB_CLIENT_H_
# define ACTION_GRAB_CLIENT_H_
# include <iosfwd>
# include <string>
# include <vector>

# include <visp/vpHomogeneousMatrix.h>
# include <visp/vpImage.h>
# include <visp/vpCameraParameters.h>

# include <LowLevelVisionSystem.hh>

# include <llvs/tools/CORBAClient.h>
# include <llvs/tools/ActionWithLLVSBase.h>

namespace trackingClient
{
  // FIXME: move this elsewhere.
  /// \brief Convert Corba image to ViSP image.
  ///
  /// Throw if the image sizes does not match.
  ///
  /// \param imageCorbaSrc Corba source image
  /// \param imageVispDest ViSP destination image
  void convertCorbaImageToVispImage(const ImageData_var& imageCorbaSrc,
				    vpImage<unsigned char>& imageVispDest);
  void convertCorbaImageToVispImage(const ImageData& imageCorbaSrc,
				    vpImage<unsigned char>& imageVispDest);


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

      explicit ActionGrab(bool rectification,
			  bool triggerMode  );
      virtual ~ActionGrab();
      virtual std::ostream& print (std::ostream& stream) const;

      virtual bool Initialize();
      virtual bool ExecuteAction();
      virtual void CleanUp();
      
      void setTriggerMode(bool atriggerMode);

      const vpCameraParameters& camera() const
      {
	return m_cam;
      }

      const image_t& image() const
	{
	  return m_image;
	}



    private:
      /// Enable trigger mode for grabbing.
      bool m_triggerMode;

      /// Pointer to the server LLVS.
      LowLevelVisionSystem_var m_LLVS;
      /// Grabbed image.
      image_t m_image;
      /// Semantic camera id.
      long m_cameraID;
      /// Image format.
      std::string m_format;
   
      //FIXME: for now this is not synchronized with server-side.
      /// Visp camera parameters
      vpCameraParameters m_cam;

    };

  std::ostream& operator <<(std::ostream& stream,
			    const ActionGrab& actionTrackingClient);
}
#endif
