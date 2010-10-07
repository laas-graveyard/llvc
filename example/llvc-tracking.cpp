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

#include <boost/program_options.hpp>

#include <llvc/action-grab.h>
#include <llvc/action-tracking-mbt.h>
#include <llvc/action-display-mbt.h>
#include <llvc/tools/indent.hh>

#include <visp/vpDisplayX.h>

#define LLVC_DEBUG(LEVEL, DATA)			\
  do {						\
    if (options.verbosity >= (LEVEL))		\
      std::cout << DATA << iendl;		\
  } while (0)

#define LLVC_DEBUG1(DATA) LLVC_DEBUG(1, DATA)
#define LLVC_DEBUG2(DATA) LLVC_DEBUG(2, DATA)
#define LLVC_DEBUG3(DATA) LLVC_DEBUG(3, DATA)


using namespace trackingClient;

namespace po = boost::program_options;

struct Options
{
  std::string modelName;
  std::string modelConfiguration;
  bool logData;
  unsigned verbosity;
};

int main (int argc, char* argv[])
{
  Options options;

  po::options_description desc ("Allowed options");

  desc.add_options ()
    ("help,h", "produce help message")

    ("model,m",
     po::value<std::string> (&options.modelName)->default_value
     ("ElectricWallFar"),
     "set the model name")

    ("model-configuration,c",
     po::value<std::string> (&options.modelConfiguration)->default_value
     ("default"),
     "set the model configuration")

    ("log-data,l",
     po::value<bool> (&options.logData)->default_value (false),
     "log images and tracking result in the logging directory")

    ("verbosity,v",
     po::value<unsigned> (&options.verbosity)->default_value (0),
     "set the verbosity level")
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
      std::cout << "This program starts a model-based tracking on the server.\n"
		<< "It needs at least a model to track as an input.\n"
		<< "By default, the electric wall at JRL Tsukuba will be used.\n"
		<< "\n"
		<< "Example:\n"
		<< "./LLVC-tracking --model=my-new-model"
		<< std::endl;
      return 0;
    }


  vpHomogeneousMatrix cMo;



  try
    {
      LLVC_DEBUG1("1. Construct Grabber");
      // Start with TRIGGER_MODE
      boost::shared_ptr<ActionGrab> clientGrab(new ActionGrab(true,true));
      assert (clientGrab);

      LLVC_DEBUG1("2. Initialize Grabber");
      clientGrab->Initialize();
      LLVC_DEBUG2(*clientGrab);

      LLVC_DEBUG1("3. Execute Grabber");
      clientGrab->ExecuteAction();

      LLVC_DEBUG1("4. Construct Display");
      ActionDisplayMbt display(clientGrab,
			       options.modelName,
			       options.modelConfiguration,
			       vpColor::red,
			       options.logData);

      if (options.verbosity > 2)
	std::cout << *clientGrab << std::endl;

      // FixME redundant with true  option in contructor...  no?
      // display.trackingClient()->setTrackingParameters("DATA", "ON");


      LLVC_DEBUG1("4. Initialize Display");
      display.Initialize();


      LLVC_DEBUG1("4.5 Initialize Display");
      // Switch to FLOW
      clientGrab->setTriggerMode(false);

      LLVC_DEBUG1("5. Execute Display");
      display.ExecuteAction();

      LLVC_DEBUG1("6. Print all" << iendl
		  << *clientGrab << iendl
		  << display);

      LLVC_DEBUG1("7. LOOP");

      unsigned int nbIter = 3000000;
      for (unsigned i = 0; i < nbIter; ++i)
	{
	  LLVC_DEBUG2("Frame " << i);
	  clientGrab->ExecuteAction();
	  display.ExecuteAction();
	  LLVC_DEBUG2(*clientGrab << iendl << display);
	}
      display.CleanUp();
      clientGrab->CleanUp();

    }
  catch(char* myexception)
    {
      std::cerr << "Exception:" << myexception << std::endl;
    }

  return 0;
}
