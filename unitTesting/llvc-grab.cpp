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

#include <llvs/tools/corba-handler.h>
#include <llvc/action-grab.h>
#include <llvc/tools/indent.hh>

#include <visp/vpDisplayX.h>

using namespace trackingClient;

int main ()
{
  trackingClient::ActionGrab client(true);

  std::cout << client << iendl;
  vpImage<unsigned char> I = client.image ();
  vpDisplayX display (I,100,100,"Grabbed image");

  client.Initialize ();
  client.ExecuteAction ();

  I = client.image ();
  vpDisplay::display(I);
  vpDisplay::flush(I);
  vpDisplay::getClick(I);

  client.CleanUp ();
  return 0;
}
