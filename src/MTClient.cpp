/*! 
 * Simple client to connect to LLVS.
 * PLEASE DO NOT ADD ANYTHING TO THIS CLIENT.
 * Motto: Keep it simple.
 *
 * Olivier Stasse, (c) JRL, 2009
*/

#include <ActionWithLLVS.h>


int main()
{
  ModelTracker::ActionWithLLVS anActionWithLLVS;
  anActionWithLLVS.SetAction(ModelTracker::ActionWithLLVS::ACTION_TRACK_AN_OBJECT);
  try
    {
      anActionWithLLVS.Initialize();
    }
  catch(const char* msg)
    {
      std::cerr << "Could not initialize component." << std::endl
		<< "Reason : " << msg << std::endl;
      anActionWithLLVS.CleanUp();
      return -1;
    }
  anActionWithLLVS.ExecuteAction();
  anActionWithLLVS.CleanUp();
}
