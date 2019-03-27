/*
 * GlobalInterruptESP8266.h
 *
 *  Created on: 17.07.2017
 *      Author: Viktor Pankraz
 */

#ifndef Basics_GlobalInterruptESP8266_H
#define Basics_GlobalInterruptESP8266_H

#include <Esp.h>

class GlobalInterrupt
{
   ////    Operations    ////

   public:

      typedef sreg_t uint32_t;

      inline static void disable() 
      {
          cli();
      }

      inline static void enable() 
      {
          sei();
      }

      inline static sreg_t lock()
      {
         return xt_rsil(15);
      }

      inline static void restore( sreg_t context )
      {
          xt_wsr_ps( context );
      }  

      inline static void waitForInterrupt() 
      {
          __asm__ __volatile__ ( "sleep" ); 
      }    
};

#endif