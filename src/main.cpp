#include <Arduino.h>
#include <Scheduler.h>
#include <DigitalPort.h>
#include <DS1820.h>
#include <SystemTime.h>
#include <Tracing/Logger.h>
#include <ApplicationTable.h>
#include <UserSignature.h>
#include <FS.h>

#include "HausBusWebServer.h"


const uint8_t debugLevel = DEBUG_LEVEL_LOW;

void printChar( char c )
{
   Serial.write( c );
}

#define getId() "HAUS-BUS.de "

void setup()
{
   // put your setup code here, to run once:
   Serial.begin( 115200 );
   Logger::setStream( printChar );
   delay( 5000 );
   DEBUG_H1( "starting..." );
   DEBUG_M2( "real flash size: ", ESP.getFlashChipRealSize() );
   DEBUG_M2( "IDE  flash size: ", ESP.getFlashChipSize() );
   if ( ESP.getFlashChipRealSize() != ESP.getFlashChipSize() )
   {
      WARN_1( "real flash size is different from coniguration in IDE!" );
   }

   DEBUG_H1( "Filesystem begin" );
   if ( !SPIFFS.begin() )
   {
      ERROR_1( "error while mounting filesystem!" );
   }

   FSInfo fs_info;
   SPIFFS.info( fs_info );
   DEBUG_M2( "Filesystem totalBytes 0x", fs_info.totalBytes );
   DEBUG_M2( " usedBytes 0x", fs_info.usedBytes );
   DEBUG_M2( " freeBytes 0x", fs_info.totalBytes - fs_info.usedBytes );
   DEBUG_L3( " (", fs_info.usedBytes * 100 / ( fs_info.totalBytes - fs_info.usedBytes ), "%) " );

   // setup the persistent memories
   ConfigurationManager::setup( Eeprom::setup( EEPROM_SIZE ), EEPROM_SIZE );
   UserSignature::setup( USER_SIGNATURE_SIZE );
   ApplicationTable::setup( APPLICATION_TABLE_SIZE );
   Scheduler::setup( MAX_JOBS );

   // start the web server
   HausBusWebServer::instance();
}

void loop()
{
   Scheduler::runJobs();
}