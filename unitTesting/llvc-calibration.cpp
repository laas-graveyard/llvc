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
#include <boost/format.hpp>

#include <llvc/action-grab.h>
#include <llvc/action-display.h>
#include <llvc/tools/indent.hh>

#include <visp/vpImageIo.h>

using namespace trackingClient;

int main ()
{
  trackingClient::ActionGrab client(false,true);

  std::cout << client << iendl;
  vpImage<unsigned char> I = client.image ();
  vpDisplayX display (I,100,100,"Grabbed image");

  client.Initialize ();
  const unsigned maxImages = 10;
  for (unsigned nImage = 0; nImage < maxImages; ++nImage)
    {
      std::cout << (boost::format("nImage = %1%") % nImage)
		<< std::endl;
      bool isFinished = false;
      while (!isFinished)
	{
	  client.ExecuteAction ();
	  I = client.image ();
	  vpDisplay::display(I);
	  vpDisplay::flush(I);
	  isFinished = vpDisplay::getClick(I, false);
	  usleep(20000);
	}
      vpImageIo::writePGM
	(I,
	 (boost::format("/tmp/llvc-calibration-%1%.pgm")
	  % nImage).str()
	 );
    }

  client.CleanUp ();
  return 0;
}
