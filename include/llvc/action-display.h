// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#ifndef ACTION_DISPLAY_H_
# define ACTION_DISPLAY_H_
# include <boost/shared_ptr.hpp>
# include <visp/vpDisplayX.h>
# include <visp/vpImage.h>
# include <llvs/tools/ActionWithLLVSBase.h>
# include <LowLevelVisionSystem.hh>
# include "llvc/action-grab.h"

namespace trackingClient
{
  /// \brief Generic display class.
  ///
  /// This class implements generic behavior for all displayers
  /// independently from the underlying algorithm (i.e. ActionTracking class).
  class ActionDisplay : public LLVClient::ActionWithLLVSBase
  {
  public:
    /// \brief Create a display from a grabbing client.
    ///
    /// \param gc shared pointer to grabbing client
    ActionDisplay(boost::shared_ptr<ActionGrab> gc);
    virtual ~ActionDisplay();

    void display();
    virtual void initClick();
    virtual bool Initialize();
    virtual bool ExecuteAction();
    virtual void CleanUp();

    virtual std::ostream& print (std::ostream& stream) const;
   private:
    /// Pointer to LLVS server.
    LowLevelVisionSystem_var m_LLVS;
  protected:
    /// Grabber pointer.
    boost::shared_ptr<ActionGrab> m_actionGrabClient;
  protected:
    /// Grabbed image copy.
    vpImage<unsigned char> m_image;
  private:
    /// ViSP display to show image.
    vpDisplayX m_display;
  };

  std::ostream& operator <<(std::ostream& stream,
			    const ActionDisplay& actionDisplay);
} // end of namespace trackingClient

#endif  // ACTION_DISPLAY_H_
