/*
 * DigitalOutput.h
 *
 * Created: 18.06.2014 14:12:56
 * Author: viktor.pankraz
 */

#ifndef Basics_DigitalOutput_H
#define Basics_DigitalOutput_H

#include "DigitalInput.h"

class DigitalOutput : public DigitalInput
{
   ////    Constructors and destructors    ////

   public:

      inline DigitalOutput( uint8_t _portNumber, uint8_t _pinNumber ) : DigitalInput( _portNumber, _pinNumber )
      {
         if ( isValid() )
         {
            configOutput();
         }
      }

      inline DigitalOutput( PortPin _portPin ) : DigitalInput( _portPin )
      {
         if ( isValid() )
         {
            configOutput();
         }
      }

      ////    Operations    ////

      inline void clear()
      {
         digitalWrite( pinNumber, inverted ? HIGH : LOW );
      }

      inline void set()
      {
         digitalWrite( pinNumber, inverted ? LOW : HIGH );
      }

      inline void toggle()
      {
         isSet() ? clear() : set();
      }
};

#endif

