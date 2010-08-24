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

  int Verbose = 0;

  try
    {
      ODEBUG3("\n1. Construct Grabber\n");
      // Start with TRIGGER_MODE
      boost::shared_ptr<ActionGrab> clientGrab(new ActionGrab(true,true));
      assert (clientGrab);

      ODEBUG3("\n2. Initialize Grabber\n");
      clientGrab->Initialize();
      if (Verbose>2)
	std::cout << *clientGrab << std::endl;

      ODEBUG3("\n3. Execute Grabber\n");
      clientGrab->ExecuteAction();

      ODEBUG3("\n4. Construct Display\n");
      ActionDisplayMbt display(clientGrab, "ElectricWallFar", "default");
      //ActionDisplayMbt display(clientGrab, "jrlLabGround", "default",
      //		       vpColor::blue, true);
      if (Verbose>2)
	std::cout << *clientGrab << std::endl;

      // FixME redundant with true  option in contructor...  no?
      // display.trackingClient()->setTrackingParameters("DATA", "ON");


      ODEBUG3("\n4. Initialize Display\n");
      display.Initialize();


      ODEBUG3("\n4.5 Initialize Display\n");
      // Switch to FLOW
      clientGrab->setTriggerMode(false);

      ODEBUG3("\n5. Execute Display\n");
      display.ExecuteAction();

      ODEBUG3("\n6. Print all\n");
      if (Verbose>2)
	std::cout << *clientGrab << iendl
		  << display << iendl;

      ODEBUG3("\n7. LOOP\n");

      unsigned int nbIter = 3000000;
      for (unsigned i = 0; i < nbIter; ++i)
	{
	  if (Verbose>2)
	    std::cout << "Frame " << i << iendl;
	  clientGrab->ExecuteAction();
	  display.ExecuteAction();
	  if (Verbose>2)
	    std::cout << *clientGrab << iendl
		      << display << iendl;
	}
      display.CleanUp();
      clientGrab->CleanUp();

    }
  catch(char *myexception)
    {
      std::cerr<< "Exception:" << myexception << std::endl;
    }

  return 0;
}
