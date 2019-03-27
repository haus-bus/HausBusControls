/*
 * UdpStream.cpp
 *
 *  Created on: 19.05.2016
 *      Author: viktor.pankraz
 */

#include "UdpStream.h"
#include <EventPkg/EventPkg.h>
#include <CriticalSection.h>
#include <ESP8266WiFi.h>

#define getId() FSTR( "UdpStream::" )
const uint8_t UdpStream::debugLevel( DEBUG_LEVEL_OFF );


bool UdpStream::init()
{
   return true;
}

IStream::Status UdpStream::genericCommand( IoStream::Command command, void* buffer )
{
   if ( command == IoStream::INIT )
   {
      DEBUG_H1( FSTR( "init..." ) );
      wifiUdp.begin( port );
      return IStream::Status::SUCCESS;
   }
   return IStream::Status::ABORTED;
}

IStream::Status UdpStream::read( void* pData, uint16_t length, EventDrivenUnit* user )
{
   int noBytes = wifiUdp.parsePacket();
   if ( noBytes )
   {
      IStream::TransferDescriptor td;
      byte packetBuffer[1024];
      DEBUG_H4( "packet of ", String( noBytes ).begin(), " received from ", wifiUdp.remoteIP().toString().begin() );
      DEBUG_L2( ':', String( wifiUdp.remotePort() ).begin() );
      {
         CriticalSection doNotInterrupt;
         wifiUdp.read( packetBuffer, noBytes ); // read the packet into the buffer
      }
      td.pData = packetBuffer;
      td.bytesTransferred = noBytes;
      if ( user )
      {
         evData( user, &td ).send();
      }
      return IStream::SUCCESS;
   }
   return IStream::NO_DATA;
}

IStream::Status UdpStream::write( void* pData, uint16_t length, EventDrivenUnit* user )
{
   DEBUG_H3( FSTR( "sending 0x" ), length, FSTR( " Bytes " ) );
   if ( WiFi.status() != WL_CONNECTED )
   {
      DEBUG_L1( "not connected" );
      return IStream::STOPPED;
   }
   IPAddress multiCastIp = WiFi.localIP();
   multiCastIp[3] = 255;
   wifiUdp.beginPacketMulticast( multiCastIp, port, WiFi.localIP() );
   wifiUdp.write( (uint8_t*)pData, length );
   DEBUG_L3( "to ", multiCastIp.toString().begin(), newTraceLine );
   if ( wifiUdp.endPacket() )
   {
      for ( uint16_t i = 0; i < length; i++ )
      {
         DEBUG_L2( ' ', ( (uint8_t*)pData )[i] );
      }
      DEBUG_L1( " successfull" );
      return IStream::SUCCESS;
   }
   DEBUG_L1( " failed" );
   return IStream::ABORTED;
}

