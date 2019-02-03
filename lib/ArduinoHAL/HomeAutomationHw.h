/*
 * HomeAutomationHw.h
 *
 *  Created on: 28.08.2014
 *      Author: Viktor Pankraz
 */

#ifndef Electronics_SystemBoards_HomeAutomationHw_H
#define Electronics_SystemBoards_HomeAutomationHw_H

#include <IStream.h>

class Checksum;

class Eeprom;

class FlashString;

class Logger;

class ModuleId;

class HomeAutomationHw
{
   public:

      static const uint32_t DBG_BAUDRATE = 115200;

      ////    Constructors and destructors    ////

      inline HomeAutomationHw()
      {
      }

      ////    Operations    ////

      static uint8_t getFirmwareId();

      static uint8_t getFckE();

      static bool getModuleId( uint8_t index, ModuleId* moduleId );

      static uint16_t readRules( uint16_t offset, void* pData, uint16_t length );

      static uint16_t writeRules( uint16_t offset, void* pData, uint16_t length );

      ////    Additional operations    ////

   protected:

      ////    Attributes    ////

      static const uint8_t debugLevel;
};

#endif
