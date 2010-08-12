//test the display class : grab & display

#include <iostream>

#include <LLVC/ActionGrabClient.h>
#include <LLVC/DisplayTrackingClient.h>

using namespace trackingClient;

int main ()
{
  boost::shared_ptr<ActionGrabClient> client_ptr(new ActionGrabClient);
  DisplayTrackingClient displayClient(client_ptr);
  client_ptr->Initialize();
  displayClient.Initialize();

  try
    {
      client_ptr->ExecuteAction ();
      displayClient.ExecuteAction ();
    }
  catch (char* str)
    {
      std::cout << str << std::endl;
    }


  displayClient.CleanUp();
  client_ptr->CleanUp ();

  return 0;
}
