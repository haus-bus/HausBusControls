/*
 * MemoryManager.h
 *
 *  Created on: 24.06.2014
 *      Author: viktor.pankraz
 */

#ifndef ArduinoHAL_MemoryManager_H
#define ArduinoHAL_MemoryManager_H

#include <stddef.h>
#include <stdint.h>

void* allocOnce( size_t size );

void getUnusedMemory( uint16_t* freeStack, uint16_t* freeHeap );

#endif
