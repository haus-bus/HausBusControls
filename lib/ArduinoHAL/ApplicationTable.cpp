/*
 * ApplicationTable.cpp
 *
 *  Created on: 18.07.2017
 *      Author: Viktor Pankraz
 */

#include "ApplicationTable.h"

ApplicationTable* ApplicationTable::theApplicationTable = NULL;

uint8_t* ApplicationTable::setup( size_t _size )
{
   static ApplicationTable ApplicationTable( _size );
   return theApplicationTable->BaseAddress;
}


uint8_t ApplicationTable::read( uintptr_t offset )
{
   if ( theApplicationTable )
   {
      return theApplicationTable->PersistentMemory::read( offset );
   }
   return 0;
}

uint16_t ApplicationTable::read( uintptr_t offset, void* pData, uint16_t length )
{
   if ( theApplicationTable )
   {
      return theApplicationTable->PersistentMemory::read( offset, pData, length );
   }
   return 0;
}

bool ApplicationTable::write( uintptr_t offset, uint8_t value )
{
   if ( theApplicationTable )
   {
      return theApplicationTable->PersistentMemory::write( offset, value );
   }
   return false;
}

uint16_t ApplicationTable::write( uintptr_t offset, const void* pData, uint16_t length )
{
   if ( theApplicationTable )
   {
      return theApplicationTable->PersistentMemory::write( offset, pData, length );
   }
   return 0;
}