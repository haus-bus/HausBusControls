/*
 * HausBusWebServer.h
 *
 *  Created on: 18.01.2019
 *      Author: Viktor Pankraz
 */

#ifndef HausBusWebServer_H
#define HausBusWebServer_H

#include <Reactive.h>
#include <IResponse.h>
#include <Protocols/Ethernet/MAC.h>
#include <ConfigurationManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


class HausBusWebServer : Reactive
{
   public:

      static const uint MAX_CONNECT_TRIES = 60;

      struct Configuration
      {
         static const uint8_t MAX_STRING_LEN = 54;

         ////    Attributes    ////

         int8_t logIn[MAX_STRING_LEN + 2];

         ////    Operations    ////

         static inline Configuration getDefault()
         {
            Configuration defaultConfiguration;
            memset( &defaultConfiguration, 0, sizeof( defaultConfiguration ) );
            return defaultConfiguration;
         }

         inline void hidePasswort()
         {
            uint8_t passPosition = strlen( (char*)logIn ) + 1;
            for ( int i = passPosition; i < MAX_STRING_LEN + 1; i++ )
            {
               logIn[i] = '*';
            }
            logIn[MAX_STRING_LEN + 1] = 0;
         }

         inline void checkAndCorrect()
         {
            for ( int i = 0; i < MAX_STRING_LEN; i++ )
            {
               if ( logIn[i] < 0 )
               {
                  logIn[i] = 0;
               }
            }
            logIn[MAX_STRING_LEN] = 0;
            logIn[MAX_STRING_LEN + 1] = 0;
         }
      };

      class EepromConfiguration : public ConfigurationManager::EepromConfigurationTmpl<Configuration>
      {
         public:
            int8_tx logIn[Configuration::MAX_STRING_LEN + 2];

            const char* getSsidString()
            {
               return (char*)logIn;
            }

            const char* getPasswordString()
            {
               return (char*)&logIn[strlen( (char*)logIn ) + 1];
            }

            void setSsidString( const char* _ssid )
            {
               Configuration conf;
               get( conf );
               for ( int i = 0; i < conf.MAX_STRING_LEN; i++ )
               {
                  conf.logIn[i] = _ssid[i];
                  if ( conf.logIn[i] == 0 )
                  {
                     break;
                  }
               }
               logIn[conf.MAX_STRING_LEN] = 0;
               logIn[conf.MAX_STRING_LEN + 1] = 0;
               set( conf );
            }

            void setPasswordString( const char* _pass )
            {
               Configuration conf;
               get( conf );
               uint8_t passPosition = strlen( (char*)conf.logIn ) + 1;
               for ( int i = passPosition; i < conf.MAX_STRING_LEN + 1; i++ )
               {
                  conf.logIn[i] = _pass[i - passPosition];
                  if ( conf.logIn[i] == 0 )
                  {
                     break;
                  }
               }
               logIn[conf.MAX_STRING_LEN + 1] = 0;
               set( conf );
            }

            inline bool isNetworkConfigured()
            {
               return strlen( getSsidString() ) > 0;
            }
      };

      class Command
      {
         public:

            enum Commands
            {
               GET_CONFIGURATION = HACF::COMMANDS_START,
               SET_CONFIGURATION,
               WAKE_UP_DEVICE,
               GET_CURRENT_IP
            };

            union Parameter
            {
               MAC mac;
               Configuration setConfiguration;
            } __attribute__( ( packed ) );

            ////    Operations    ////

            uint8_t getCommand() const;

            inline Parameter& getParameter()
            {
               return parameter;
            }

            void setCommand( uint8_t p_command );

            void setParameter( const Parameter& p_parameter );

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
               CURRENT_IP
            };

            union Parameter
            {
               Configuration configuration;
               uint32_t ip;
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

            void setCurrentIp();

            ////    Attributes    ////

         private:

            Parameter params;
      };

      enum SubStates
      {
         REBOOT,
         START_CONNECT,
         WAIT_CONNECT,
         CONNECTED,
         OPEN_AP,
         RUN_AP,
      };

      static HausBusWebServer& instance();

      bool notifyEvent( const Event& event );

      void onRoot();
      void onConfig();
      void onSubmit();
      void onUpdate();
      void onRestorFactory();

   protected:
      inline void setConfiguration( EepromConfiguration* _config )
      {
         configuration = _config;
      }

      bool handleRequest( HACF* message );

      void wakeUpDevice( const MAC& mac );

      HausBusWebServer();
      ~HausBusWebServer();

   private:
      static const uint8_t debugLevel;

      bool firstConnect;

      EepromConfiguration* configuration;

      uint connectTries;

      ESP8266WebServer server;

      MDNSResponder mdns;
};

#endif