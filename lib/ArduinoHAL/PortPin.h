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

      ////    Constructors and destructors    ////

      inline PortPin( uint8_t _portNumber, uint8_t _pinNumber ) : pinNumber( _pinNumber ), portNumber( _portNumber )
      {
      }

      ////    Operations    ////

      inline void configInput()
      {
         pinMode( pinNumber, INPUT );
      }

      void configOutput()
      {
         pinMode( pinNumber, OUTPUT );
      }

      inline bool isValid() const
      {
         return true;
      }

      inline void enablePullup()
      {
         digitalWrite( pinNumber, HIGH );
      }

      inline void enablePulldown()
      {
         digitalWrite( pinNumber, LOW );
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
         bool pinState = digitalRead( pinNumber );
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
