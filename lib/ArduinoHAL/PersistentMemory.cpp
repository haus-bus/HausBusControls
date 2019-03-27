/*
 * PersistentMemory.cpp
 *
 *  Created on: 18.07.2017
 *      Author: Viktor Pankraz
 */

#include "PersistentMemory.h"
#include <FS.h>
#include <Tracing/Logger.h>
#include <string.h>

const uint8_t PersistentMemory::debugLevel( DEBUG_LEVEL_OFF );

PersistentMemory::PersistentMemory( const char* name, size_t _size ) : fileName( name )
{
   DEBUG_H2( " setup for size: 0x", _size );
   BaseAddress = new uint8_t[_size];
   if ( BaseAddress != NULL )
   {
      size = _size;
      if ( SPIFFS.exists( fileName ) )
      {
         DEBUG_H1( " read into local buffer" );
         File theFile = SPIFFS.open( fileName, "r" );
         theFile.read( BaseAddress, theFile.size() );
         theFile.close();
      }
      else
      {
         DEBUG_M1( " not exists, start with empty memory" );
         memset( BaseAddress, 0xFF, _size );
      }
      return;
   }
   else
   {
      ERROR_2( getId(), " failed to allocate memory for buffer" );
   }
   size = 0;
}


uint8_t PersistentMemory::read( uintptr_t offset )
{
   // adjust memory mapped access
   if ( offset >= (uintptr_t)BaseAddress )
   {
      offset -= (uintptr_t)BaseAddress;
   }
   DEBUG_H2( " read at 0x", offset );
   if ( !isAddressInRange( offset ) )
   {
      return false;
   }
   DEBUG_M2( ' ', BaseAddress[offset] );
   return BaseAddress[offset];
}

uint16_t PersistentMemory::read( uintptr_t offset, void* pData, uint16_t length )
{
   // adjust memory mapped access
   if ( offset >= (uintptr_t)BaseAddress )
   {
      offset -= (uintptr_t)BaseAddress;
   }
   DEBUG_H4( " read at 0x", offset, ' ', length );
   if ( !isAddressInRange( offset ) || !isAddressInRange( offset + length ) )
   {
      return 0;
   }
   memcpy( pData, &BaseAddress[offset], length );
   DEBUG_M1( ' ' );
   for ( uint16_t i = 0; i < length; i++ )
   {
      DEBUG_L2( BaseAddress[offset + i], ' ' );
   }
   return length;
}

bool PersistentMemory::write( uintptr_t offset, uint8_t value )
{
   // adjust memory mapped access
   if ( offset >= (uintptr_t)BaseAddress )
   {
      offset -= (uintptr_t)BaseAddress;
   }
   DEBUG_H2( " write at 0x", offset );

   if ( !isAddressInRange( offset ) )
   {
      return false;
   }

   if ( BaseAddress[offset] != value )
   {
      BaseAddress[offset] = value;
      commit();
      DEBUG_M2( ' ', BaseAddress[offset] );
   }
   return true;
}

uint16_t PersistentMemory::write( uintptr_t offset, const void* pData, uint16_t length )
{
   // adjust memory mapped access
   if ( offset >= (uintptr_t)BaseAddress )
   {
      offset -= (uintptr_t)BaseAddress;
   }
   DEBUG_H4( " write at 0x", offset, ' ', length );
   if ( !isAddressInRange( offset ) || !isAddressInRange( offset + length ) )
   {
      return false;
   }

   if ( memcmp( &BaseAddress[offset], pData, length ) )
   {
      memcpy( &BaseAddress[offset], pData, length );
      commit();
      DEBUG_M1( ' ' );
      for ( uint16_t i = 0; i < length; i++ )
      {
         DEBUG_L2( BaseAddress[offset + i], ' ' );
      }
   }
   return length;
}

bool PersistentMemory::isAddressInRange( uintptr_t address )
{
   if ( address > size )
   {
      ERROR_3( getId(), " isOutOfRange 0x", address );
      return false;
   }
   return true;
}

bool PersistentMemory::commit()
{
   DEBUG_H1( " save data" );
   File theFile = SPIFFS.open( fileName, "w" );
   if ( theFile )
   {
      size_t writtenBytes = theFile.write( BaseAddress, size );
      theFile.close();
      if ( writtenBytes != size )
      {
         ERROR_4( "Could only write 0x", writtenBytes, " / 0x", size );
         return false;
      }
      return true;
   }
   ERROR_2( getId(), " could not be opened! Data not saved" );
   return false;
}
