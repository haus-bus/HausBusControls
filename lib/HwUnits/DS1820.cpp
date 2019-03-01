/*
 * DS1820.cpp
 *
 *  Created on: 28.08.2014
 *      Author: Viktor Pankraz
 */

#include "DS1820.h"
#include <Security/Crc8.h>

uint8_t DS1820::numOfInstances( 0 );

bool DS1820::selfPowered( true );

const uint8_t DS1820::debugLevel( DEBUG_LEVEL_OFF );

void DS1820::scanAndCreateDevices( PortPin _owPin )
{
   OneWire ow( _owPin.getPinNumber() );

   // wait 1ms to stabilize the line
   SystemTime::waitMs( 1 );
   DEBUG_M4( FSTR( "1-Wire port: " ), _owPin.getPortNumber(), FSTR( "pin: " ), _owPin.getPinNumber() );
   ow.reset_search();
   DS1820::RomCode newRomCode;
   while ( ow.search( (uint8_t*)&newRomCode ) )
   {
      DEBUG_M1( FSTR( "0x" ) );
      for ( uint8_t i = 0; i < sizeof( romCode ); i++ )
      {
         DEBUG_L1( ( (uint8_t* )&newRomCode )[i] );
      }

      if ( Crc8::hasError( (uint8_t* )&newRomCode, sizeof( newRomCode ) ) )
      {
         ERROR_1( "CRC is not valid!" );
         continue;
      }

      if ( DS1820::isSensor( newRomCode.family ) )
      {
         DEBUG_L1( FSTR( "->DS18X20" ) );
         new DS1820( ow, newRomCode );
      }
      else
      {
         DEBUG_L1( FSTR( "->UNKNOWN" ) );
      }
   }
}

DS1820::DS1820( const OneWire& _hardware, const DS1820::RomCode& _romCode ) :
   hardware( _hardware ), romCode( _romCode )
{
   setId( ( ClassId::TEMPERATURE << 8 ) | ++numOfInstances );
}

bool DS1820::isSelfPowered()
{
   hardware.reset();
   // hardware.sendCommand( READ_POWER_SUPPLY, (uint8_t*) &romCode );
   hardware.select( (uint8_t*) &romCode );
   hardware.write( READ_POWER_SUPPLY );

   selfPowered = hardware.read_bit();

   hardware.reset();
   return selfPowered;
}

bool DS1820::isSensor( uint8_t familiyCode )
{
   return ( ( familiyCode == DS18B20_ID ) || ( familiyCode == DS18S20_ID ) );
}

bool DS1820::notifyEvent( const Event& event )
{
   if ( event.isEvWakeup() )
   {
      run();
   }
   else if ( event.isEvMessage() )
   {
      return handleRequest( event.isEvMessage()->getMessage() );
   }
   return false;
}

DS1820::HwStatus DS1820::readMeasurement()
{
   uint8_t sp[SCRATCHPAD_SIZE];

   hardware.reset();
   hardware.select( (uint8_t*) &romCode );
   hardware.write( READ, !selfPowered );

   for ( uint8_t i = 0; i < SCRATCHPAD_SIZE; i++ )
   {
      sp[i] = hardware.read();
   }
   if ( Crc8::hasError( sp, SCRATCHPAD_SIZE ) )
   {
      return CRC_FAILTURE;
   }
   notifyNewValue( convertToCelsius( sp ) );

   return OK;
}

void DS1820::run()
{
   if ( inStartUp() )
   {
      setConfiguration( ConfigurationManager::getConfiguration<EepromConfiguration>( id ) );
      if ( configuration )
      {
         isSelfPowered();
         startMeasurement();
         SET_STATE_L1( RUNNING );
         setSleepTime( SystemTime::S );
      }
      else
      {
         terminate();
         ErrorMessage::notifyOutOfMemory( id );
         return;
      }
   }
   else if ( inIdle() )
   {
      setSleepTime( NO_WAKE_UP );
   }
   else if ( inRunning() )
   {
      Response event( getId() );
      uint8_t error = readMeasurement();
      if ( error )
      {
         event.setErrorCode( error );
         event.queue();
      }
      error = startMeasurement();
      if ( error )
      {
         event.setErrorCode( error );
         event.queue();
      }
      setSleepTime( SystemTime::S* configuration->reportTimeBase );
   }

}

DS1820::HwStatus DS1820::startMeasurement( bool allSensors )
{
   if ( hardware.reset() )
   {
      hardware.select( (uint8_t*) &romCode );
      hardware.write( CONVERT_T, !selfPowered );
      return OK;
   }
   else
   {
      ERROR_1( "OW not idle on startMeasurement" );
      return START_FAIL;
   }
}

BaseSensorUnit::Status DS1820::convertToCelsius( uint8_t* scratchPad )
{
   uint16_t measurement = scratchPad[0];        // LSB
   measurement |= ( (uint16_t) scratchPad[1] ) << 8; // MSB

   // only work on 12bit-base
   if ( romCode.family == DS18S20_ID ) // 9 -> 12 bit if 18S20
   {
      // Extended measurements for DS18S20
      measurement &= (uint16_t) 0xfffe; // Discard LSB , needed for later extended precicion calc
      measurement <<= 3;// Convert to 12-bit , now degrees are in 1/16 degrees units
      measurement += ( 16 - scratchPad[6] ) - 4;// Add the compensation , and remember to subtract 0.25 degree (4/16)
   }

   // clear undefined bits for B != 12bit
   if ( romCode.family == DS18B20_ID ) // check resolution 18B20
   {
      uint8_t i = scratchPad[DS18B20_CONF_REG];
      if ( ( i & DS18B20_12_BIT ) == DS18B20_12_BIT )
      {
      }
      else if ( ( i & DS18B20_11_BIT ) == DS18B20_11_BIT )
      {
         measurement &= ~( DS18B20_11_BIT_UNDF );
      }
      else if ( ( i & DS18B20_10_BIT ) == DS18B20_10_BIT )
      {
         measurement &= ~( DS18B20_10_BIT_UNDF );
      }
      else
      { // if ( (i & DS18B20_9_BIT) == DS18B20_9_BIT ) {
         measurement &= ~( DS18B20_9_BIT_UNDF );
      }
   }

   Status status;
   status.value = static_cast<int8_t>( measurement >> 4 );
   uint8_t fracture = (uint8_t) ( measurement & 0x000F );
   status.centiValue = 0;
   if ( fracture & 0x8 )
   {
      status.centiValue += 50;
   }
   if ( fracture & 0x4 )
   {
      status.centiValue += 25;
   }
   if ( fracture & 0x2 )
   {
      status.centiValue += 13;
   }
   if ( fracture & 0x1 )
   {
      status.centiValue += 6;
   }

   return status;
}
