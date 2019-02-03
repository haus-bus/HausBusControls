/*
 * Eeprom.cpp
 *
 *  Created on: 18.07.2017
 *      Author: Viktor Pankraz
 */

#include "Eeprom.h"

Eeprom* Eeprom::theEeprom = NULL;

uint8_t* Eeprom::setup( size_t _size )
{
   static Eeprom Eeprom( _size );
   return theEeprom->BaseAddress;
}


uint8_t Eeprom::read( uintptr_t offset )
{
   if ( theEeprom )
   {
      return theEeprom->PersistentMemory::read( offset );
   }
   return 0;
}

uint16_t Eeprom::read( uintptr_t offset, void* pData, uint16_t length )
{
   if ( theEeprom )
   {
      return theEeprom->PersistentMemory::read( offset, pData, length );
   }
   return 0;
}

bool Eeprom::write( uintptr_t offset, uint8_t value )
{
   if ( theEeprom )
   {
      return theEeprom->PersistentMemory::write( offset, value );
   }
   return false;
}

uint16_t Eeprom::write( uintptr_t offset, const void* pData, uint16_t length )
{
   if ( theEeprom )
   {
      return theEeprom->PersistentMemory::write( offset, pData, length );
   }
   return 0;
}