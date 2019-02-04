/*
 * PortPin.h
 *
 *  Created on: 17.07.2017
 *      Author: Viktor Pankraz
 */

#ifndef PortPin_H
#define PortPin_H

#include <Arduino.h>

class PortPin
{

   public:

      static const uint8_t invalidId = 0xF;

      ////    Constructors and destructors    ////
      inline PortPin() : pinNumber( invalidId ), portNumber( invalidId )
      {
      }

      inline PortPin( uint8_t _portNumber, uint8_t _pinNumber ) : pinNumber( _pinNumber ), portNumber( _portNumber )
      {
      }

      inline PortPin( uint8_t _pinId ) : pinNumber( _pinId & 0xF ), portNumber( _pinId >> 4 )
      {
      }

      ////    Operations    ////

      inline void configInput()
      {
         pinMode( getPinId(), INPUT );
      }

      void configOutput()
      {
         pinMode( getPinId(), OUTPUT );
      }

      inline bool isValid() const
      {
         return portNumber != invalidId;
      }

      inline void enablePullup()
      {
         digitalWrite( getPinId(), HIGH );
      }

      inline void enablePulldown()
      {
         digitalWrite( getPinId(), LOW );
      }

      inline uint8_t isInverted()
      {
         return inverted;
      }

      inline void setInverted( bool _inverted )
      {
         inverted = _inverted;
      }

      inline uint8_t isSet() const
      {
         bool pinState = digitalRead( getPinId() );
         return pinState ^ inverted;
      }

      ////    Additional operations    ////

      inline uint8_t getPinNumber() const
      {
         return pinNumber;
      }

      inline uint8_t getPortNumber() const
      {
         return portNumber;
      }

      inline uint8_t getPinMask() const
      {
         return ( 1 << pinNumber );
      }

      inline uint8_t getPinId() const
      {
         return ( portNumber << 4 | pinNumber );
      }

      inline void setPinNumber( uint8_t p_pinNumber )
      {
         pinNumber = p_pinNumber;
      }

      inline void setPortNumber( uint8_t p_portNumber )
      {
         portNumber = p_portNumber;
      }

      ////    Attributes    ////
   protected:

      uint8_t pinNumber : 4;

      uint8_t portNumber : 4;

      uint8_t inverted;
};

#endif
