//test the display class : grab & display

#include <iostream>

#include <llvc/action-grab.h>
#include <llvc/action-display.h>

using namespace trackingClient;

int main ()
{
  boost::shared_ptr<ActionGrab> client_ptr(new ActionGrab);
  ActionDisplay displayClient(client_ptr);
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
