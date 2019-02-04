/*
 * DigitalPort.h
 *
 *  Created on: 28.08.2014
 *      Author: Viktor Pankraz
 */

#ifndef HwUnits_DigitalPort_H
#define HwUnits_DigitalPort_H

#include "HwUnits.h"
#include "PortPin.h"
#include <ConfigurationManager.h>

class DigitalPort : public Reactive
{
   public:

      enum ErrorCodes
      {
         PINFUNCTION_NOT_SUPPORTED = 0x10
      };

      class Configuration
      {
         public:

            static const uint8_t MAX_PINS = 8;

            ////    Attributes    ////

            uint8_t pinFunction[MAX_PINS];

            ////    Operations    ////

            static inline Configuration getDefault()
            {
               Configuration defaultConfiguration;
               memset( &defaultConfiguration, 0xFF, sizeof( defaultConfiguration ) );
               return defaultConfiguration;
            }

            inline void checkAndCorrect()
            {
            }
      } __attribute__( ( packed ) );

      class EepromConfiguration : public ConfigurationManager::EepromConfigurationTmpl<Configuration>
      {
         public:

            uint8_tx pinFunction[Configuration::MAX_PINS];

            uint16_tx reserve;

            inline uint8_t getPinFunction( uint8_t idx )
            {
               if ( idx < Configuration::MAX_PINS )
               {
                  return pinFunction[idx];
               }
               return 0xFF;
            }

            inline void setPinFunction( uint8_t idx, uint8_t _pinFunction )
            {
               if ( idx < Configuration::MAX_PINS )
               {
                  pinFunction[idx] = _pinFunction;
               }
            }
      };

      class Command
      {
         public:

            enum Commands
            {
               GET_CONFIGURATION = HACF::COMMANDS_START,
               SET_CONFIGURATION,
            };

            union Parameter
            {
               Configuration setConfiguration;
            } __attribute__( ( packed ) );

            ////    Operations    ////

            inline Parameter& getParameter()
            {
               return parameter;
            }

            ////    Additional operations    ////

            inline uint8_t getCommand() const
            {
               return command;
            }

            inline void setCommand( uint8_t p_command )
            {
               command = p_command;
            }

            inline void setParameter( Parameter p_parameter )
            {
               parameter = p_parameter;
            }

            ////    Attributes    ////

            uint8_t command;

            Parameter parameter;
      };

      class Response : public IResponse
      {
         public:

            enum Responses
            {
               CONFIGURATION = HACF::RESULTS_START,
            };

            union Parameter
            {
               Configuration configuration;
            } __attribute__( ( packed ) );

            ////    Constructors and destructors    ////

            inline Response( uint16_t id ) :
               IResponse( id )
            {
            }

            inline Response( uint16_t id, const HACF& message ) :
               IResponse( id, message )
            {
            }

            ////    Operations    ////

            inline Parameter& getParameter()
            {
               return *reinterpret_cast<Parameter*>( IResponse::getParameter() );
            }

            Parameter& setConfiguration();

            ////    Attributes    ////

         private:

            Parameter params;
      };

      ////    Constructors and destructors    ////

      inline DigitalPort( uint8_t portNumber, uint8_t _notUsablePins = 0 ) :
         notUseablePins( _notUsablePins )
      {
         configuration = NULL;
         setId( ( ClassId::DIGITAL_PORT << 8 ) | ( ( portNumber + 1 ) << 4 ) );
      }

      ////    Operations    ////

      inline uint8_t isPinUsable( uint8_t pinMask )
      {
         return !( pinMask & notUseablePins );
      }

      inline void setPins( uint8_t index, PortPin portPin )
      {
         if ( index < Configuration::MAX_PINS )
         {
            pins[index] = portPin;
         }
      }

      virtual bool notifyEvent( const Event& event );

      void run();

   protected:

      bool handleRequest( HACF* message );

   private:

      void clearPinFunction();

      void configureHw();

      ////    Additional operations    ////

   public:

      inline uint8_t getNotUseablePins() const
      {
         return notUseablePins;
      }

      inline void setNotUseablePins( uint8_t p_notUseablePins )
      {
         notUseablePins = p_notUseablePins;
      }

      inline void setConfiguration( EepromConfiguration*  _config )
      {
         configuration = _config;
      }


   protected:

      inline static const uint8_t getDebugLevel()
      {
         return debugLevel;
      }

   private:

      ////    Attributes    ////

   public:



   protected:

      static const uint8_t debugLevel;

   private:

      uint8_t notUseablePins;

      PortPin pins[Configuration::MAX_PINS];

      ////    Relations and components    ////

   protected:

      EepromConfiguration* configuration;
};

#endif
