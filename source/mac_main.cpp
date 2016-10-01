//--------------------------------------------------------------------------------------------------
// JC CLOCK
// Jason Colman 2016
//--------------------------------------------------------------------------------------------------

#include <iostream>

#if defined(MACOSX)
#include <unistd.h>
#endif

#if defined(WIN32)
#include <Windows.h>
#endif

#include "jc_clock_main.h"

int main()
{
  setup();
  
  while (true)
  {
std::cout << "Loop!\n";

    loop();
  
    const int ms = 1000;
 
#if defined(MACOSX)
  // Sleep for ms millisecs
  useconds_t usecs = ms * 1000;
  usleep(usecs);
#endif

#ifdef WIN32
  ::Sleep(ms);
#endif 
  }
}

