/*
 * MemoryManager.cpp
 *
 *  Created on: 28.08.2014
 *      Author: Viktor Pankraz
 */

#include "MemoryManager.h"
#include <CriticalSection.h>


void* allocOnce( size_t size )
{
   return new uint8_t[size];
}

void getUnusedMemory( uint16_t* _freeStack, uint16_t* _freeHeap )
{
   // reference might not be 2-Byte aligned, access bytewise
   uint8_t* ptr = reinterpret_cast<uint8_t*>( _freeStack );
   ptr[0] = 0;
   ptr[1] = 0;
   ptr = reinterpret_cast<uint8_t*>( _freeHeap );
   ptr[0] = 0;
   ptr[1] = 0;
}
