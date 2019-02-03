/*
 * CriticalSection.h
 *
 *  Created on: 16.01.2019
 *      Author: Viktor Pankraz
 */

#ifndef CriticalSection_H
#define CriticalSection_H

#include <Arduino.h>

class CriticalSection
{
   ////    Constructors and destructors    ////

   public:

      inline CriticalSection()
      {
         noInterrupts();
      }

      inline ~CriticalSection()
      {
         interrupts();
      }

      ////    Attributes    ////

   private:
};

#endif

