/*
 * SystemTime.cpp
 *
 *  Created on: 28.08.2014
 *      Author: Viktor Pankraz
 */

#include <Arduino.h>
#include <SystemTime.h>
#include <Time/Calender.h>

int8_t SystemTime::ticksPerSecondAdjustment( 0 );

void SystemTime::init( ClockSources cs, uint16_t frequency )
{

}

SystemTime::time_t SystemTime::now()
{
   return millis();
}

SystemTime::time_t SystemTime::since( const SystemTime::time_t& lastTime )
{
   time_t elapsedTime, timestamp = now();

   // was there an overun
   if ( lastTime > timestamp )
   {
      elapsedTime = static_cast<time_t>( -1 ) - lastTime + timestamp;

      // it is not very common that the overrun was more than MAX_TIME/2 ago
      // assume in that case, that the lastTime is a timestamp in the (near) future
      if ( elapsedTime > static_cast<time_t>( -1 ) / 2 )
      {
         // the timestamp 'lastTime' was not elapsed
         elapsedTime = 0;
      }
   }
   else
   {
      elapsedTime = ( timestamp - lastTime );
   }

   return elapsedTime;
}

void SystemTime::waitMs( uint16_t ms )
{
   delay( ms );
}

void SystemTime::set( SystemTime::time_t value )
{

}

