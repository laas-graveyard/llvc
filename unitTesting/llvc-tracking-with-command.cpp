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
#include <llvc/action-tracking-with-command.h>
#include <llvc/action-display-with-command.h>
#include <llvc/tools/indent.hh>

#include <visp/vpDisplayX.h>
#include <visp/vpParseArgv.h>

#define GETOPTARGS  "h:k:n:"

using namespace trackingClient;

void usage(const char* name)
{
  std::cout << std::endl
	    << "\t" << name << " [-n <nb desired poses>] [-v <verbosity level>] [-h <help>]" 
	    << "\t" <<  "nb desired poses : "
    	    << std ::endl
	    << "\t\t" <<  "number of poses the robot has to reach"
    	    << std::endl 
	    << "\t" <<  "verbosity type : "
    	    << std ::endl
	    << "\t\t" <<  "0 : no verbosity"
    	    << std ::endl
	    << "\t\t" <<  ">2 :  verbosity"
    	    <<std::endl; 
}

bool readOption (int argc, const char ** argv,  unsigned &nbDesPose,int & Verbose)
{

  const char *optarg ="";
  int   c;
  std::string option="";

  while ((c = vpParseArgv::parse(argc, argv, GETOPTARGS, &optarg)) > 1) 
    {
      switch(c)
	{
	case 'n':
	  {
	    nbDesPose=atoi(optarg);
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
  int Verbose = 3;
  unsigned nbDesPose = 1;
  // read user option
  if(!readOption (argc, argv, nbDesPose,Verbose))
    return -1;
  ODEBUG("Verbosity mode is:" << Verbose );
  ODEBUG3("Nb desired posed is:" << nbDesPose );
  std::string configurationName = "default"; 
  vpHomogeneousMatrix cMo;

  try
    {
      ODEBUG3("\n1. Construct Grabber\n");
      boost::shared_ptr<ActionGrab> clientGrab(new ActionGrab(true,true));
      ODEBUG3("Assert Grab\n");
      assert (clientGrab);
      ODEBUG3("Grab ok\n");


      ODEBUG3("\n2. Initialize Grabber\n");
      clientGrab->Initialize();
      clientGrab->setTriggerMode(false);
      
      if (Verbose>2)
	std::cout << *clientGrab << std::endl;
      
      ODEBUG3("\n3. Execute Grabber\n");
      clientGrab->ExecuteAction();
      
      ODEBUG3("\n4. Construct Display\n");
      ActionDisplayWithCommand display
	(
	 clientGrab, 
	 "ElectricWallFar", 
	 configurationName,
	 nbDesPose,
	 vpColor::blue, 
	 true);
      
      if (Verbose>2)
	std::cout << *clientGrab 
		  << std::endl
		  << std::endl
		  <<  display << iendl;
         
      unsigned iter=0;  
      
      //Test go to next and display
      while(display.trackingClient()->nextDesiredPose())
      	{
        iter++;
        std::cout  <<"--------" << iter 
      	     << iendl
      	     << display 
      	     << iendl;
         }
 
      ODEBUG3("\n4. Initialize Display\n");
      display.Initialize();


      ODEBUG3("\n6. Execute Display\n");
      display.ExecuteAction();

     
      //ODEBUG3("\n7. LOOP\n");
      // unsigned int nbIter = 3000000;
      //for (unsigned i = 0; i < nbIter; ++i)
      //{
      //  if (Verbose>2)
      //    std::cout << "Frame " << i << iendl;
      //  clientGrab->ExecuteAction();
      //  display.ExecuteAction();
      //  if (Verbose>2)
      //    std::cout << *clientGrab << iendl
      //	      << display << iendl;
      //}
      display.CleanUp();
      clientGrab->CleanUp();

    }
  catch(char *myexception)
    {
      std::cerr<< "Exception:" << myexception << std::endl;
    }

  return 0;
}
