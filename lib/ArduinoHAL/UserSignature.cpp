/*
 * UserSignature.cpp
 *
 *  Created on: 18.07.2017
 *      Author: Viktor Pankraz
 */

#include "UserSignature.h"

UserSignature* UserSignature::theUserSignature = NULL;

uint8_t* UserSignature::setup( size_t _size )
{
   static UserSignature UserSignature( _size );
   return theUserSignature->BaseAddress;
}


uint8_t UserSignature::read( uintptr_t offset )
{
   if ( theUserSignature )
   {
      return theUserSignature->PersistentMemory::read( offset );
   }
   return 0;
}

uint16_t UserSignature::read( uintptr_t offset, void* pData, uint16_t length )
{
   if ( theUserSignature )
   {
      return theUserSignature->PersistentMemory::read( offset, pData, length );
   }
   return 0;
}

bool UserSignature::write( uintptr_t offset, uint8_t value )
{
   if ( theUserSignature )
   {
      return theUserSignature->PersistentMemory::write( offset, value );
   }
   return false;
}

uint16_t UserSignature::write( uintptr_t offset, const void* pData, uint16_t length )
{
   if ( theUserSignature )
   {
      return theUserSignature->PersistentMemory::write( offset, pData, length );
   }
   return 0;
}