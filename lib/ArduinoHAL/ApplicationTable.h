/*
 * ApplicationTable.h
 *
 *  Created on: 18.07.2017
 *      Author: Viktor Pankraz
 */

#ifndef Peripherals_ApplicationTable_H
#define Peripherals_ApplicationTable_H

#include <SwFramework.h>
#include "PersistentMemory.h"

class ApplicationTable : public PersistentMemory
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

      inline ApplicationTable( size_t _size ) : PersistentMemory( "ApplicationTable", _size )
      {
         theApplicationTable = this;
      }

      ////    Attributes    ////

   protected:

      static ApplicationTable* theApplicationTable;
};

#endif

