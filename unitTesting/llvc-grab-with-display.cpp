// Copyright (C) 2010 by Claire Dune, Thomas Moulard, CNRS.
//
// This file is part of the LLVC.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
// See the COPYING file for more information.

#include <iostream>

#include <llvc/action-grab.h>
#include <llvc/action-display.h>
#include <llvc/tools/indent.hh>

using namespace trackingClient;

int main ()
{
  boost::shared_ptr<ActionGrab> client(new ActionGrab);
  ActionDisplay displayClient(client);
  client->Initialize();
  displayClient.Initialize();
  
  std::cout << *client << iendl
	    << displayClient << iendl;

  client->ExecuteAction ();
  displayClient.ExecuteAction ();

  displayClient.CleanUp();
  client->CleanUp ();

  return 0;
}
