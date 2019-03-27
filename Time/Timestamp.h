/*
 * Timestamp.h
 *
 *  Created on: 28.08.2014
 *      Author: Viktor Pankraz
 */

#ifndef SwFramework_Time_Timestamp_H
#define SwFramework_Time_Timestamp_H


#include "Time.h"
#include "DefaultTypes.h"
#include "SystemTime.h"

class Timestamp
{
   ////    Constructors and destructors    ////

   public:

      inline Timestamp() :
         time( SystemTime::now() )
      {
      }

      ////    Operations    ////

      inline bool elapsed( const SystemTime::time_t& value )
      {
         return ( value < SystemTime::since( time ) );
      }

      inline SystemTime::time_t since()
      {
         return SystemTime::since( time );
      }

      inline bool isValid()
      {
         return time != 0;
      }

      inline void reset()
      {
         time = 0;
      }

      inline void setNow()
      {
         time = SystemTime::now();
      }

      inline void operator +=( int32_t _time )
      {
         time += _time;
      }

      inline SystemTime::time_t get()
      {
         return time;
      }

      ////    Attributes    ////

   private:

      SystemTime::time_t time;
};


#endif