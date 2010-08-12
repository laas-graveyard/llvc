// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include <cassert>
#include <iostream>

#include <llvc/action-grab.h>
#include <llvc/action-tracking-mbt.h>
#include <llvc/action-display-mbt.h>
#include <llvc/tools/indent.hh>

#include <visp/vpDisplayX.h>

using namespace trackingClient;

int main ()
{
  vpHomogeneousMatrix cMo;

  boost::shared_ptr<ActionGrab> clientGrab
    (new ActionGrab);
  assert (clientGrab);

  clientGrab->Initialize();
  clientGrab->ExecuteAction();

  ActionDisplayMbt display(clientGrab, "ElectricWallFar", "default");
  display.Initialize();
  display.ExecuteAction();

  for (unsigned i = 0; i < 4; ++i)
    {
      std::cout << "Frame " << i << iendl;
      clientGrab->ExecuteAction();
      display.ExecuteAction();
      std::cout << *clientGrab << iendl
		<< display << iendl;
    }

  display.CleanUp();
  clientGrab->CleanUp();
  return 0;
}
