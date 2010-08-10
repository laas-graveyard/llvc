/*! ----------------------------------------------------
 *  Copyright 2010, CNRS-AIST JRL
 * 
 *  \brief  Model tracker client for LVLS
 *  \author Claire Dune & Stephane Embarki
 *  \edit   Clement Petit
 *  \creation 15/06/2010
 * ---------------------------------------------------- */

/*! Includes system */
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <signal.h>
#include <cstdlib>
#include <string.h>

/*! Includes application specifics */
#include "ActionWithLLVS.h"
#include "ModelTrackerInterface.hh"

/*! Includes visp */
#include "opencv/cvver.h"
#include "visp/vpConfig.h"
#include "visp/vpImageIo.h"
#include "visp/vpDisplayX.h"
#include "visp/vpXmlParserCamera.h"
#include "visp/vpCameraParameters.h"



/*! Includes debug */
#include <llvs/tools/Debug.h>

bool useKalmanFilter =true;

/* ---------------------------------------------------
 * Initialization/Destruction
 * --------------------------------------------------- */

ModelTracker::ActionWithLLVS::ActionWithLLVS()
{
}

ModelTracker::ActionWithLLVS::~ActionWithLLVS()
{
}

/* ---------------------------------------------------
 * Action interface
 * --------------------------------------------------- */

bool
ModelTracker::ActionWithLLVS::Initialize()
  throw(const char*)
{
  std::cout << "ActionWithLLVS::Initialize()" << std::endl;
  m_LLVS = GetServicePort<LowLevelVisionSystem, LowLevelVisionSystem_var>();
  std::cout << "Initialization finished. " << std::endl;
  return true;
}

bool
ModelTracker::ActionWithLLVS::ExecuteAction()
{
  switch(m_InstanceAction)
    {
    case ACTION_GRAB_AN_IMAGE:
      return GrabAnImageAndWriteItOnAFile();
      break;
    case ACTION_TRACK_AN_OBJECT:
      return TrackAnObject();
      break;
    }
  return false;
}

void
ModelTracker::ActionWithLLVS::CleanUp()
{
}


/* ---------------------------------------------------
 * Local actions
 * --------------------------------------------------- */

bool
ModelTracker::ActionWithLLVS::GrabAnImageAndWriteItOnAFile()
{
  
  return true;
}

 
bool 
ModelTracker::ActionWithLLVS::TrackAnObject()
{
 
  HRP2CameraSeq_var lcameras;
  CORBA::Long CameraNb=0,r;
  char * Format = CORBA::string_dup("RGB");
  std::cout << "Entered bool ActionWithLLVS::TrackAnObject() "<< std::endl;

  try 
    {
      m_LLVS->SetSynchronizationMode(LowLevelVisionSystem::SYNCHRO_TRIGGER);
    }
  catch(...)
    {
      std::cerr << "Error on Set SynchronizationMode:" << std::endl;
      return false;
    }


  std::cout << "Setting the IEEE process. "<<std::endl;
  m_LLVS->SetAProcessParameterAndValue("IEEE1394 Image grabbing","Format","RGB");
 
  std::cout << "Starting the process "<< std::endl;
  m_LLVS->StartProcess("vispUndistordedProcess");
  m_LLVS->StartProcess("IEEE1394 Image grabbing");

   
  ImageData_var anImage  = new ImageData;  
  vpImage<unsigned char> Ivisp;
  Ivisp.resize(240,320);

  unsigned int nbCheckPoint=0;

  cout<< "How many checkpoint for the trajectory :";
  cin>>nbCheckPoint;

  int reload=1;
  
  cout<< endl<< "Do you want to reaload the last "<<nbCheckPoint<<" checkpoints(0(no)/1(yes)):";
  cin>>reload;
  cout<< endl;

  nmbtTracking trackerClient;
  vpCameraParameters cam;
  vpDisplayX displayg(Ivisp,0,600,"Image grey undistorded");

  unsigned int cp = 0;

  if(reload==1)
    cp=nbCheckPoint+1;
      
  for(cp; cp<nbCheckPoint+2; ++cp)
    {

      m_LLVS->TriggerSynchro();
      sleep(1);
      try 
	{
	  r =m_LLVS->getRectifiedImage(CameraNb,anImage,Format);
	  
	}
      catch( const CORBA::MARSHAL &e)
	{
	  std::cerr << "Still a problem with the marshalling."  << std::endl;
	  return false;
	}
      sleep(1);
      

      unsigned char *pt = Ivisp.bitmap;

      for(unsigned int l=0;l<anImage->octetData.length();++l)
	{
	  *pt++ = anImage->octetData[l];
	}	
      
 

      vpDisplay::display(Ivisp);
	
      
      // ------------- TRACKER INIT-------------------//

      // trackerClient.setDisplayMovingEdges(true);
     
      if(cp==nbCheckPoint+1)
	{
	  cout<<"  ------------- The robot is at the start point -------------"<<endl;
	  initTrackerServer(trackerClient,Ivisp,cam);
	}
      else
	{
	  cout<<" ------------- The robot is at the point number"<<cp<<" -------------"<<endl;
	  std::ofstream filecdMo;
	  char  cdMofile[128];
	  sprintf(cdMofile,"../cdMo/cdMo%d.txt",cp);
	  filecdMo.open (cdMofile);

	  //TODO :: check if the file is correctly open
	  initTrackerAndSave(filecdMo,Ivisp,cam );
	  filecdMo.close();

	  cout<<endl<<" ------------- Put the robot at the next check point"<<endl<< "wait for keyboard eventclick on the keyboard when ready ------------- "<<endl;
	  vpDisplay::getKeyboardEvent (Ivisp,true);

	}
     }


  //Send parmeters to the robot from a file
  std::vector< std::pair<string,string> > CCLParam;
  SetParameters("ComputeControlLawProcess",CCLParam);

  if (useKalmanFilter)
    m_LLVS->StartProcess("KalmanOnNMBTProcess");
  else
    m_LLVS->StartProcess("nmbtTrackingProcess");

  m_LLVS->StartProcess("CircularModelTrackerData");

  // Change Synchronization mode to FLOW
  m_LLVS->SetSynchronizationMode(LowLevelVisionSystem::SYNCHRO_FLOW);
  // active the change of mode
  m_LLVS->TriggerSynchro();
  sleep(2);
  //run the  mode FLOW
  m_LLVS->TriggerSynchro();

  std::pair<long,long> TimeStamp;

  vpImage<unsigned char> Ivisp2;
  Ivisp2.resize(240,320);
  
  vpDisplayX display2(Ivisp2,350,600,"Image Server");
  
  std::ofstream filecMo;
  filecMo.open ("../tmp/cMo.txt");


  vpHomogeneousMatrix cMo;

  ModelTrackerInterface_var MTI;
  MTI = m_LLVS->getModelTracker();

  bool CCLPstarted=false;

  for (cp=0; cp<nbCheckPoint+1; ++cp)
    { 
  
      vpImagePoint p(10*cp+10,10); 
      vpDisplay::getKeyboardEvent (Ivisp,true);
      
      char  imageText[128];
      sprintf(imageText,"CLICK OK ! Go to position %d",cp+1);
      vpDisplay::displayCharString (Ivisp,p,imageText,vpColor::green); 	
      vpDisplay::flush(Ivisp);
      
      std::ifstream ifilecdMo;
      char  cdMofile[128];
      sprintf(cdMofile,"../cdMo/cdMo%d.txt",cp);
      ifilecdMo.open (cdMofile);
      
      vpHomogeneousMatrix cdMo;
      cdMo.load(ifilecdMo);
      ifilecdMo.close();
      cout<<cdMo<<endl;
      
      ModelTrackerInterface::HomogeneousMatrix HM;
      
      for (int i = 0; i<4;++i)
	{
	  for (int j = 0; j<4;++j)
	    {
	      HM.cMo[i][j]=cdMo[i][j];
	    }
	}
      MTI->SetcdMo(HM);

      if (!CCLPstarted)
	{
	  m_LLVS->StartProcess("ComputeControlLawProcess");
	  cout<<"ComputeControlLawProcess started"<<endl;
	  CCLPstarted=true;
	}
    
      bool lcontinue=true;

      while (lcontinue)
	{  
	  
	  /* Keybooard on the windows use for the initialisation
	     Start the compute law
	     the second one stop all processes*/
	  if(vpDisplay::getKeyboardEvent (Ivisp,false))
	    {
	      lcontinue=false;
	    }
	  
	  // Wait for not asking data to often
	  usleep(30000);
	  
	  GetBufferData(MTI,Ivisp2,cMo,TimeStamp);

	  vpDisplay::display(Ivisp2);
	  trackerClient.display(Ivisp2,cMo,cam,vpColor::orange,2);


	  SaveData(filecMo,Ivisp2,cMo,TimeStamp);
 
	  vpHomogeneousMatrix cdMc;

	  cdMc=cdMo*cMo.inverse();
	  
	  vpPoseVector Error(cdMc);

	  vpColVector ErrorTest(3);
	  ErrorTest[0]=Error[0];
	  ErrorTest[1]=Error[2];
	  ErrorTest[2]=Error[4];

	  if(ErrorTest.infinityNorm()<0.1)
	    {
	      lcontinue=false;
	      cout<<"Finish with te Error : "<<Error.t();
	    }
	  	    
	}/* End while loop*/
    }/* End for loop*/
  filecMo.close();

  m_LLVS->StopProcess("ComputeControlLawProcess");
  m_LLVS->StopProcess("IEEE1394 Image grabbing");
  m_LLVS->StopProcess("vispUndistordedProcess");

  if (useKalmanFilter)
    m_LLVS->StopProcess("KalmanOnNMBTProcess");
  else
    m_LLVS->StopProcess("nmbtTrackingProcess");

  m_LLVS->StopProcess("CircularModelTrackerData");
 
  // Wait for a click on the initialisation window to stop
  std::cout << "USER : wait for the user to click on the view 1 to continue" <<std::endl;
  vpDisplay::getClick(Ivisp);

  return true;

}

/*! Initialize the trackers on the client and on the server . */
int ModelTracker::ActionWithLLVS::initTrackerServer(nmbtTracking& atrackerClient,
					      vpImage<unsigned char> &aIvisp,
					      vpCameraParameters &cam )
{

 

  // TODO : Change this, the camera parameter shouldn't be hard written
  std::string m_CamParamPath;

  vpXmlParserCamera  m_ParserCam;
  m_CamParamPath="./data/ViSP/hrp2CamParam/hrp2.xml";
  m_ParserCam.parse(cam,
		    m_CamParamPath.c_str(),
		    "cam1394_3_rectif",
		    vpCameraParameters::perspectiveProjWithoutDistortion,
		    320,
		    240);

  std::cout<<"camera parameter:\n"<< cam;
  std::cout<<m_CamParamPath.c_str()<<std::endl;

  //Send parmeters to the robot from a file
  std::vector< std::pair<string,string> > trackerParam;


  if (useKalmanFilter)
    SetParameters("KalmanOnNMBTProcess",trackerParam);
  else
    SetParameters("nmbtTrackingProcess",trackerParam);


  // Set vpMe parameters
  vpMe me ;
  me.setMaskSize(atof(trackerParam[0].second.c_str())) ;
  me.setRange(atof (trackerParam[1].second.c_str())) ;
  me.setThreshold(atof (trackerParam[2].second.c_str()));
  me.setSampleStep(atof (trackerParam[3].second.c_str())) ;
  me.setMu1(atof (trackerParam[4].second.c_str()));
  me.setMu2(atof (trackerParam[5].second.c_str()));
  
  std::string vrmlPath=trackerParam[6].second;


  // New tracker
  atrackerClient.loadModel(vrmlPath.c_str());
  atrackerClient.setCameraParameters(cam);
  atrackerClient.setMovingEdge(me);

  // remove path extension
  int vrmlPathSize=vrmlPath.size();
  std::string modelPath=vrmlPath.substr(0, vrmlPathSize-4);

  // init the tracking with clicks
  atrackerClient.initClick(aIvisp, modelPath.c_str()); 

  // 3 tracks to verify the initialisation
  atrackerClient.track(aIvisp);
  atrackerClient.track(aIvisp);
  atrackerClient.track(aIvisp);

  // create an initial pose
  vpHomogeneousMatrix cMo;
  atrackerClient.getPose(cMo);
  std::cout << "cMo \n"<< cMo<<std::endl ;

  //Diplay the same image 
  vpDisplay::display(aIvisp);	

  //Diplau the tracked pose
  atrackerClient.display(aIvisp, cMo, cam, vpColor::red,2); 


  // Set the cMo for the tracker on LLVS
  ModelTrackerInterface_var MTI;
  MTI = m_LLVS->getModelTracker();

  ModelTrackerInterface::HomogeneousMatrix HM;

  for (int i = 0; i<4;++i)
    {
      for (int j = 0; j<4;++j)
	{
	  HM.cMo[i][j]=cMo[i][j];
	}
    }

  // send cMo to the tracking process
  // and to the compute control law process 
  MTI->SetcMo(HM);     
 
  ODEBUG3("End initTrackerServer.");
  return 0;
}

/*! Initialize the trackers on the client and on the server . */
int ModelTracker::ActionWithLLVS::initTrackerAndSave(ofstream& aofstream,
						     vpImage<unsigned char> &aIvisp,
						     vpCameraParameters &cam )
{

  nmbtTracking atrackerClient;
  // TODO : Change this, the camera parameter shouldn't be hard written
  std::string m_CamParamPath;

  vpXmlParserCamera  m_ParserCam;
  m_CamParamPath="./data/ViSP/hrp2CamParam/hrp2.xml";
  m_ParserCam.parse(cam,
		    m_CamParamPath.c_str(),
		    "cam1394_3_rectif",
		    vpCameraParameters::perspectiveProjWithoutDistortion,
		    320,
		    240);

  //std::cout<<"camera parameter:\n"<< cam;
  //std::cout<<m_CamParamPath.c_str()<<std::endl;

  //Send parmeters to the robot from a file
  std::vector< std::pair<string,string> > trackerParam;


  if (useKalmanFilter)
    SetParameters("KalmanOnNMBTProcess",trackerParam,false);
  else
    SetParameters("nmbtTrackingProcess",trackerParam,false);


  // Set vpMe parameters
  vpMe me ;
  me.setMaskSize(atof(trackerParam[0].second.c_str())) ;
  me.setRange(atof (trackerParam[1].second.c_str())) ;
  me.setThreshold(atof (trackerParam[2].second.c_str()));
  me.setSampleStep(atof (trackerParam[3].second.c_str())) ;
  me.setMu1(atof (trackerParam[4].second.c_str()));
  me.setMu2(atof (trackerParam[5].second.c_str()));
  
  std::string vrmlPath=trackerParam[6].second;


  // New tracker
  atrackerClient.loadModel(vrmlPath.c_str());
  atrackerClient.setCameraParameters(cam);
  atrackerClient.setMovingEdge(me);

  // remove path extension
  int vrmlPathSize=vrmlPath.size();
  std::string modelPath=vrmlPath.substr(0, vrmlPathSize-4);

  bool tracker_track_success;
  do
    {
      // init the tracking with clicks
      atrackerClient.initClick(aIvisp, modelPath.c_str()); 
    
      vpDisplay::display(aIvisp);
      // refine the tracking using a first stp of optimisation 
      tracker_track_success = true;
      try
	{
	  // 3 tracks to verify the initialisation
	  atrackerClient.track(aIvisp);
	  //  atrackerClient.track(aIvisp);
	  //atrackerClient.track(aIvisp);  	
	}
      catch(...)
	{
	  
	  std::cerr << std::endl;
	  std::cerr << "-----    -----   Failed with exception \""  << "\"     -----    -----" << std::endl;
	  std::cerr << std::endl;
	  tracker_track_success = false;
	}
    }
  while(tracker_track_success == false);
  

  // create an initial pose
  vpHomogeneousMatrix cMo;
  atrackerClient.getPose(cMo);
  std::cout << "cMo \n"<< cMo<<std::endl ;

  //Diplay the same image 
  vpDisplay::display(aIvisp);	

  //Diplau the tracked pose
  atrackerClient.display(aIvisp, cMo, cam, vpColor::red,2); 

  cMo.save(aofstream);
      
  return 0;
}





/*! Get the data from the bufer on LLVS. */
int ModelTracker::ActionWithLLVS::GetBufferData(ModelTrackerInterface_var aMTI,
						vpImage<unsigned char> &aIvisp, 
						vpHomogeneousMatrix &acMo,
						std::pair <long , long > &TimeStamp)
{

  ModelTrackerInterface::DebugInfoObject_var DIOv;
  ModelTrackerInterface::DebugInfoObject DIO;
  
  ODEBUG("Before getting the information from the serveur.");
  aMTI->GetDebugInfoObject(DIOv);
  
  ODEBUG("After getting the information from the serveur.");
  DIO=DIOv;

  unsigned char *pt2 = aIvisp.bitmap;
  for(unsigned int l=0;l< DIO.anImgData.octetData.length();l++)
    {
      *pt2++ = DIO.anImgData.octetData[l];
    }
  
  
  for (int i = 0; i<4;++i)
    {
      for (int j = 0; j<4;++j)
	{
	  acMo[i][j]= DIO.aData.cMo[i][j];
	}
    }
  
  TimeStamp=make_pair(DIO.anImgData.longData[0],
		      DIO.anImgData.longData[1]);

  return 0;
}



/*! Save the image and the value in a tmp directory. */
int ModelTracker::ActionWithLLVS::SaveData(std::ofstream &afilecMo,
					   vpImage<unsigned char> &aIvisp, 
					   vpHomogeneousMatrix &acMo,
					   std::pair <long , long > &TimeStamp)
{
  static unsigned int linc=0;
  
  afilecMo<<linc<<std::endl;
       
  afilecMo<<TimeStamp.first<<"."<<TimeStamp.second<<std::endl;
    
  afilecMo<<acMo<<std::endl;
       

  char Buffer[128];
  sprintf(Buffer,"../tmp/image%06d.pgm",linc);
  
  fstream aofstream;
  aofstream.open(Buffer,ofstream::out);
  if (aofstream.is_open())
    {  
	   
      aofstream << "P5" <<std::endl;
      aofstream << "# TimeStmamp : "<<TimeStamp.first<<"."<<TimeStamp.second<<std::endl;
      aofstream << aIvisp.getWidth()  << " " << aIvisp.getHeight ()  << std::endl;
      aofstream << "255" <<std::endl;
      
      for(unsigned int l=0;l< aIvisp.getNumberOfPixel();l++)
	{
	  aofstream <<aIvisp.bitmap[l];
	}
      aofstream.close();
    }

  sprintf(Buffer,"../tmp/image-res-%06d.ppm",linc++);
       
  vpImage<vpRGBa> Ivispcolor;
  vpDisplay::getImage(aIvisp,Ivispcolor);
  vpImageIo::writePPM(Ivispcolor,Buffer) ;
  
  return 0;
}


/*! Initialize the parameter from a file and fill a vector for a process. */
int ModelTracker::ActionWithLLVS::SetParameters(string aprocessName, std::vector< std::pair<string,string> > &ParamList,bool sendIt)
{

  // string operator
  ostringstream tmp_stream;
  tmp_stream.str("");
  tmp_stream<<"./data/"<<aprocessName<<".init";
  std::string initPath =  tmp_stream.str() ;

  ifstream aifstream;
  aifstream.open(initPath.c_str());

  if (aifstream.is_open())
    {
      int lnbParam=0;
      aifstream>>lnbParam;

      ParamList.clear();
      ParamList.resize(lnbParam);
 
      for(int i=0 ; i<lnbParam ; ++i )
	{
	  aifstream>>ParamList[i].first;
	  aifstream>>ParamList[i].second;
	  
	  ODEBUG(ParamList[i].first<<" : "<<ParamList[i].second);
	}
      
      if (sendIt)
	{
	  for(unsigned int i=0; i<ParamList.size();++i)
	    {
	      m_LLVS->SetAProcessParameterAndValue(aprocessName.c_str(),
						   ParamList[i].first.c_str(),
						   ParamList[i].second.c_str());
	    }
	}//if 
    }
  else
    {
      cout<< "COULD NOT OPEN :"<<initPath<<endl;
      return -1;
    }

  return 0;
}
