/*
 * HomeAutomationHw.cpp
 *
 *  Created on: 28.08.2014
 *      Author: Viktor Pankraz
 */

#include "HomeAutomationHw.h"
#include "UserSignature.h"
#include "ApplicationTable.h"
#include <Release.h>
#include <Security/ModuleId.h>

const ModuleId moduleId = { "$MOD$ ESP-8266 ",
                            512 * 1024,
                            Release::MAJOR,
                            Release::MINOR,
                            CONTROLLER_ID,
                            0 };

bool HomeAutomationHw::getModuleId( uint8_t index, ModuleId* _moduleId )
{
   memcpy( _moduleId, &moduleId, sizeof( moduleId ) );
   return true;
}


uint8_t HomeAutomationHw::getFirmwareId()
{
   return UserSignature::read( 0 );
}

uint8_t HomeAutomationHw::getFckE()
{
   return UserSignature::read( 1 );
}

uint16_t HomeAutomationHw::readRules( uint16_t offset, void* pData, uint16_t length )
{
   return ApplicationTable::read( offset, pData, length );
}

uint16_t HomeAutomationHw::writeRules( uint16_t offset, void* pData, uint16_t length )
{
   if ( length )
   {
      return ApplicationTable::write( offset, pData, length );
   }
   return length;
}
