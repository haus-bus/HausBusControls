/*
 * Basics.cpp
 *
 * Created: 18.06.2014 14:12:55
 * Author: viktor.pankraz
 */

#include <Arduino.h>

bool isRunning()
{
   // needed by scheduler to decide if Scheduler::runJobs is the main loop
   return false;
}

void notifyBusy()
{
   /* ToDo
      TRACE_PORT_SET( TR_IDLE_PIN );
      greenLed.set();
    */
}

void notifyIdle()
{
   /* ToDo
      TRACE_PORT_CLEAR( TR_IDLE_PIN );
      greenLed.clear();

    #ifndef _DEBUG_
      WatchDog::reset();
    #endif
    */
}
