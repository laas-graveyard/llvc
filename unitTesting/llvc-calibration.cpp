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
#include <boost/program_options.hpp>

#include <llvc/action-grab.h>
#include <llvc/action-display.h>
#include <llvc/tools/indent.hh>

#include <visp/vpImageIo.h>

using namespace trackingClient;

namespace po = boost::program_options;

struct Options
{
  unsigned nImages;
  std::string outputDirectory;
  bool rectify;
};


int main (int argc, char* argv[])
{
  Options options;

  po::options_description desc ("Allowed options");

  desc.add_options ()
    ("help,h", "produce help message")

    ("nImages,n",
     po::value<unsigned> (&options.nImages)->default_value (10),
     "how many images should be captured")

    ("output,o",
     po::value<std::string> (&options.outputDirectory)->default_value ("/tmp"),
     "where should the captured images be stored")

    ("rectify,r",
     po::value<bool> (&options.rectify)->default_value (false),
     "should the image be rectified")
    ;

  po::variables_map vm;
  try
    {
      po::store (po::parse_command_line (argc, argv, desc), vm);
      po::notify (vm);
    }
  catch (po::error& error)
    {
      std::cerr << "Error while parsing argument: "
		<< error.what () << std::endl;
      return 1;
    }

  if (vm.count ("help"))
    {
      std::cout << desc << "\n";
      std::cout << "This program captures several images and exit.\n"
		<< "It is well suited to be used during a camera calibration process.\n"
		<< "\n"
		<< "Example:\n"
		<< "./LLVC-calibration"
		<< std::endl;
      return 0;
    }


  trackingClient::ActionGrab client(false,true);

  std::cout << "Client:" << iendl
	    << client << iendl;

  vpImage<unsigned char> I = client.image ();
  vpDisplayX display (I,100,100,"Grabbed image");

  client.Initialize ();
  const unsigned maxImages = options.nImages;
  for (unsigned nImage = 0; nImage < maxImages; ++nImage)
    {
      std::cout << (boost::format("nImage = %1%") % nImage)
		<< iendl;
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
	 (boost::format("%1%/llvc-calibration-%02d.pgm")
	  % options.outputDirectory
	  % nImage).str()
	 );
    }

  client.CleanUp ();
  return 0;
}
