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
#include <sstream>

#include <llvc/action-grab.h>
#include <llvc/action-tracking-mbt.h>
#include <llvc/action-display-mbt.h>
#include <llvc/tools/indent.hh>

#include <visp/vpDisplayX.h>
#include <visp/vpImage.h>

using namespace std;
using namespace trackingClient;


int selectRangeOnline(const double& Zcourant)
{
  int rangeMax   = 6;
  int rangeMin   = 2;//3;
  double pente   = -1;//-3;
  double origine = 6;//15;

  // Change the range online !!!
  int rangeCourant;
  rangeCourant = int(origine+pente*Zcourant);

  if(rangeCourant>rangeMax) rangeCourant=rangeMax;
  else if(rangeCourant<rangeMin) rangeCourant =rangeMin;

  return rangeCourant;
}   






int main ()
{
  vpHomogeneousMatrix cMo;

  //int Verbose = 3;

  
  std :: string imOPath		= "/home/dune/devel/openrobots/var/log";
  std :: string configIPath     = "data/model";
  std :: string object 	 	= "blue-rectangle" ;
  std :: string base 	 	= "image-" ;
  std :: string extension	= "pgm";
  std :: string dataOPath	= "/home/dune/devel/openrobots/var/log/pose.dat";
  std::ostringstream tmp_stream;
  tmp_stream << configIPath << "/" << object<<"/"<< object ;
  configIPath = tmp_stream.str();

  try
    {

      ODEBUG3("Start the grabbing");
      // Start with TRIGGER_MODE
      boost::shared_ptr<ActionGrab> clientGrab(new ActionGrab(true,false));
      assert (clientGrab);
      clientGrab->Initialize();
      clientGrab->ExecuteAction();

      vpImage<unsigned char> I;
      I = clientGrab->image();
      vpDisplayX display (I,100,100,"Grabbed image");

    
      vpCameraParameters cam(clientGrab->camera());
      vpMbtTracker tracker;
      tracker.setCameraParameters(cam) ;
      vpMe me ;

      me.setSizeMask(3) ;
      me.setRange(4) ;
      me.setThreshold(100) ;
      me.setSampleStep(3); 
      me.setMu1(0.7);
      me.setMu2(0.7);

      tracker.setMovingEdge(me) ;
      tmp_stream.str("");
      tmp_stream << configIPath << ".wrl";
      tracker.loadModel(tmp_stream.str().c_str());
      tracker.setDisplayMovingEdges (true);
      //ActionDisplayMbt display(clientGrab, "blue-rectangle", "default");
      //display.Initialize();
      //display.ExecuteAction();
      //if (Verbose>2)
      //    std::cout << *clientGrab << iendl
      //	      << display << iendl;
      
      cout << " Click when ready" << endl;
      bool userClick=false;
      while(!userClick)
	{
	  clientGrab->ExecuteAction();
	  I = clientGrab->image();
	  vpDisplay::display(I);
	  vpDisplay::flush(I);
	  userClick=vpDisplay::getClick(I,false);
	}

      tracker.initClick(I,configIPath.c_str());
      vpDisplay::display(I);
      // refine the tracking using a first stp of optimisation
      tracker.track(I) ;
      // display on the view
      vpHomogeneousMatrix cMo;
      tracker.getPose(cMo);
      tracker.display(I, cMo, cam, vpColor::green,2);
      vpDisplay::displayFrame(I,cMo,cam, 0.05, vpColor::none);
      vpDisplay::flush(I) ;
      // display the first pose estimation
      std::cout << "cMo init : " << std::endl << cMo << std::endl;
      
      cout<< "Change the range and tracker" <<endl;
      double Z = cMo[2][3];
      me.setRange(selectRangeOnline(Z));
      tracker.setMovingEdge(me) ;


      char number[50] ;
      bool echec = false;
      while(!echec)
	{
         
          clientGrab->ExecuteAction();
	  I = clientGrab->image();
          vpDisplay::display(I);
	  vpDisplay::flush(I);
	  try
	    {
	      tracker.track(I) ;
	    }
	  catch(...)
	    {
	      echec = true ;
	      cout << "tracking has failed" <<endl;
	    }
    
	  tracker.getPose(cMo) ;
	  //change range with distance for the next step
	  Z = cMo[2][3];
	  me.setRange(selectRangeOnline(Z));
	  tracker.setMovingEdge(me) ;
	  cout << "---->range "  << selectRangeOnline(Z) <<endl;


    

	  if(echec)
	    {
	      sprintf(number,"Error tracking") ;
	      vpDisplay::displayCharString(I,100,100,number, vpColor::red) ;
	      vpDisplay::flush(I) ;
	      vpDisplay::getClick(I) ;
	    }
	  else
	    {
	      tracker.display(I,cMo,cam,vpColor::blue,2);
	      vpDisplay::displayFrame(I,cMo,cam, 0.05, vpColor::none);
	       vpDisplay::flush(I) ;
	    }
         

	  //display.ExecuteAction();
	  //if (Verbose>2)
	  //  std::cout << *clientGrab << iendl
	  //	      << display << iendl;
	}
      // display.CleanUp();
      clientGrab->CleanUp();
    }
  catch(char *myexception)
    {
      std::cerr<< "Exception:" << myexception << std::endl;
    }

  return 0;
}
