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
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#include <llvc/action-grab.h>
#include <llvc/action-tracking-kalman.h>
#include <llvc/action-display-kalman.h>
#include <llvc/tools/indent.hh>

#include <visp/vpDisplayX.h>
#include <visp/vpParseArgv.h>

#define GETOPTARGS  "h:k:"

using namespace trackingClient;

void usage(const char* name)
{
  std::cout << std::endl
	    << "\t" << name << "  [-k <kalman type>] [-v <verbosity level>] [-h <help>]" 
	    << std::endl
	    << std::endl 
	    << "\t" <<  "verbosity type : "
    	    << std ::endl
	    << "\t\t" <<  "0 : no verbosity"
    	    << std ::endl
	    << "\t\t" <<  ">2 :  verbosity"
    	    << std ::endl
	    << "\t" <<  "kalman type : " 
	    << std ::endl
	    << "\t\t"<< "acceleration or a : state model with constant acceleration" 
	    << std::endl
	    << "\t\t"<< "velocity or v : state model with constant velocity" 
	    <<std::endl; 
}

bool readOption (int argc, const char ** argv,std::string & kalmanType, int & Verbose)
{

  const char *optarg ="";
  int   c;
  std::string option="";

  while ((c = vpParseArgv::parse(argc, argv, GETOPTARGS, &optarg)) > 1) 
    {
      switch(c)
	{
        case 'k':
	  {	
	    option=optarg;
            std::string kalman = option.substr(0,1);
   	    if (kalman == "v"|kalman == "V")
	      kalmanType = "kalmanV";
	    else if(kalman == "a"|kalman == "A")
	      kalmanType = "kalmanA";
	  
	    break;
	}
	case 'v':
	  {
	    Verbose = atoi(optarg);
	    break;
	  }
	case '?':
	case 'h':
	  {
            usage(argv[0]);
	    return false;
	  }
	default:
	  {
	    std::cerr << c << "is not an option. "<< std::endl;
	    usage(argv[0]);
	    return false;
	  }    

	}

    }
  return true;

}

int main (int argc, const char **argv)
{

  // set default parameters
  std::string configurationName="kalmanA";
  int Verbose = 0;
  // read user option
  if(!readOption (argc, argv,configurationName, Verbose))
    return -1;
  
  ODEBUG3("Configuration mode is:" << configurationName );
  ODEBUG3("Verbosity mode is:" << Verbose );


  vpHomogeneousMatrix cMo;

  

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
      ActionDisplayKalman display(clientGrab, "ElectricWallFar", configurationName,
			       vpColor::blue, true);
      if (Verbose>2)
	std::cout << *clientGrab << std::endl
		  << "Display ..." <<std::endl 
		  << display << iendl;
      display.trackingClient()->setTrackingParameters("DATA_LOG", "ON");


      ODEBUG3("\n4. Initialize Display\n");
      display.Initialize();


      ODEBUG3("\n5.5 Initialize Display\n");
      // Switch to FLOW
      clientGrab->setTriggerMode(false);

      ODEBUG3("\n6. Execute Display\n");
      display.ExecuteAction();

     
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
