/*
 * GlobalInterrupt.h
 *
 *  Created on: 17.07.2017
 *      Author: Viktor Pankraz
 */

#ifndef Basics_GlobalInterrupt_H
#define Basics_GlobalInterrupt_H

#ifdef __AVR__
#include <GlobalInterruptAVR.h>
#elif defined ESP8266
#include <GlobalInterruptESP8266.h>
#else
#warning "Unkown environment, please extend support here."

class GlobalInterrupt
{
   ////    Operations    ////

   public:

      typedef sreg_t unsigned char;

      inline static sreg_t lock()
      {
         return 0;
      }

      inline static void restore( sreg_t context )
      {
      }

      inline static void enable()
      {
      }

      inline static void disable()
      {
      }

      inline static void waitForInterrupt()
      {
      }


};
#endif
#endif

