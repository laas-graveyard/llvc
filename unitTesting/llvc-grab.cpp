#include <iostream>

#include <LLVC/ActionGrabClient.h>
#include <LLVC/DisplayTrackingClient.h>

int main ()
{
  trackingClient::ActionGrabClient client;
  trackingClient::DisplayTrackingClient displayClient;

  std::cout << client << std::endl;
  //std::cout << displayClient << std::endl;

  vpImage<unsigned char> I = client.image ();
  vpDisplayX display (I,100,100,"Grabbed image");

  client.Initialize ();
  try
    {
      client.ExecuteAction ();
    }
  catch (char* str)
    {
      std::cout << str << std::endl;
    }
  I = client.image ();
  vpDisplay::display(I);
  vpDisplay::flush(I);
  vpDisplay::getClick(I);

  client.CleanUp ();
}
