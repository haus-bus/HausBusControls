/*
 * UdpStream.h
 *
 *  Created on: 19.05.2016
 *      Author: viktor.pankraz
 */



#ifndef UDP_STREAM_H_
#define UDP_STREAM_H_

#include <IoStream.h>
#include <WiFiUDP.h>

class UdpStream : public IoStream
{
   public:

      ////    Constructors and destructors    ////

      inline UdpStream( uint16_t _port = 9 ) : port( _port )
      {

      }
      ////    Operations    ////

   public:

      IStream::Status genericCommand( Command command, void* buffer );

      IStream::Status read( void* pData, uint16_t length, EventDrivenUnit* user = 0 );

      IStream::Status write( void* pData, uint16_t length, EventDrivenUnit* user = 0 );

   protected:

      bool init();

   private:

      ////    Attributes    ////

   protected:

      static const uint8_t debugLevel;

      WiFiUDP wifiUdp;

      uint16_t port;

};

#endif /* UDP_STREAM_H_ */
