/*
 * Eeprom.h
 *
 *  Created on: 18.07.2017
 *      Author: Viktor Pankraz
 */

#ifndef Peripherals_Eeprom_H
#define Peripherals_Eeprom_H

#include <SwFramework.h>
#include "PersistentMemory.h"

class Eeprom : public PersistentMemory
{
   public:

      static uint8_t* setup( size_t _size );

      static void erase();

      static uint8_t read( uintptr_t offset );

      static uint16_t read( uintptr_t offset, void* pData, uint16_t length );

      static bool write( uintptr_t offset, uint8_t value );

      static uint16_t write( uintptr_t offset, const void* pData, uint16_t length );

   protected:

   private:

      inline Eeprom( size_t _size ) : PersistentMemory( "Eeprom", _size )
      {
         theEeprom = this;
      }

      ////    Attributes    ////

   protected:

      static Eeprom* theEeprom;
};

#endif

