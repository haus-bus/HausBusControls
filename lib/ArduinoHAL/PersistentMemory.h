/*
 * PersistentMemory.h
 *
 *  Created on: 18.07.2017
 *      Author: Viktor Pankraz
 */

#ifndef Peripherals_PersistentMemory_H
#define Peripherals_PersistentMemory_H

#include <SwFramework.h>
#include <FS.h>

class PersistentMemory
{
   public:

      PersistentMemory( const char* name, size_t _size );

      void erase();

      uint8_t read( uintptr_t offset );

      uint16_t read( uintptr_t offset, void* pData, uint16_t length );

      bool write( uintptr_t offset, uint8_t value );

      uint16_t write( uintptr_t offset, const void* pData, uint16_t length );

   protected:

      bool commit();

      inline char* getId()
      {
         return fileName.begin();
      }

   private:

      bool isAddressInRange( uint32_t address );

      ////    Attributes    ////

   protected:

      static const uint8_t debugLevel;

      uint8_t* BaseAddress;

      size_t size;

      String fileName;
};

#endif

