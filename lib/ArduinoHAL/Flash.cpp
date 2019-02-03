/*
 * Flash.cpp
 *
 *  Created on: 18.07.2017
 *      Author: Viktor Pankraz
 */

#include "Flash.h"

Flash* Flash::theFlash = NULL;

uint8_t* Flash::setup( size_t _size )
{
   static Flash Flash( _size );
   return theFlash->BaseAddress;
}


uint8_t Flash::read( uintptr_t offset )
{
   if ( theFlash )
   {
      return theFlash->PersistentMemory::read( offset );
   }
   return 0;
}

uint16_t Flash::read( uintptr_t offset, void* pData, uint16_t length )
{
   if ( theFlash )
   {
      return theFlash->PersistentMemory::read( offset, pData, length );
   }
   return 0;
}

bool Flash::write( uintptr_t offset, uint8_t value )
{
   if ( theFlash )
   {
      return theFlash->PersistentMemory::write( offset, value );
   }
   return false;
}

uint16_t Flash::write( uintptr_t offset, const void* pData, uint16_t length )
{
   if ( theFlash )
   {
      return theFlash->PersistentMemory::write( offset, pData, length );
   }
   return 0;
}