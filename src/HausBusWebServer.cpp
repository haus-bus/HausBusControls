/*
 * HausBusWebServer.cpp
 *
 *  Created on: 18.01.2019
 *      Author: Viktor Pankraz
 */

#include "HausBusWebServer.h"
#include "UdpStream.h"
#include <ErrorMessage.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <HomeAutomation.h>
#include <Gateway.h>


const uint8_t HausBusWebServer::debugLevel( DEBUG_LEVEL_LOW | DEBUG_STATE_L3 );

void handleRoot()
{
   HausBusWebServer::instance().onRoot();
}
void handleConfig()
{
   HausBusWebServer::instance().onConfig();
}
void handleSubmit()
{
   HausBusWebServer::instance().onSubmit();
}
void handleUpdate()
{
   HausBusWebServer::instance().onUpdate();
}
void handleRestorFactory()
{
   HausBusWebServer::instance().onRestorFactory();
}

HausBusWebServer::HausBusWebServer() : connectTries( 0 ), server( 80 )
{
   setId( ( ClassId::WIFI << 8 ) | 1 );
   server.on( "/", handleRoot );
   server.on( "/config", handleConfig );
   server.on( "/submit", handleSubmit );
   server.on( "/update", handleUpdate );
   server.on( "/restoreFactory", handleRestorFactory );

   // do not save data to flash
   WiFi.persistent( false );
}

HausBusWebServer::~HausBusWebServer()
{
}

HausBusWebServer& HausBusWebServer::instance()
{
   static HausBusWebServer theServer;
   return theServer;
}

bool HausBusWebServer::notifyEvent( const Event& event )
{
   bool consumed = false;
   if ( event.isEvWakeup() )
   {
      if ( inStartUp() )
      {
         setConfiguration( ConfigurationManager::getConfiguration<EepromConfiguration>( id ) );
         if ( configuration )
         {
            server.begin();
            SET_STATE_L1( RUNNING );
            DEBUG_M2( "SSID: ", configuration->getSsidString() );
            DEBUG_M2( "PASS: ", configuration->getPasswordString() );
            if ( configuration->isNetworkConfigured() )
            {
               SET_STATE_L2( START_CONNECT );
            }
            else
            {
               SET_STATE_L2( OPEN_AP );
            }
         }
         else
         {
            terminate();
            ErrorMessage::notifyOutOfMemory( id );
         }
      }
      else if ( inRunning() )
      {
         if ( inSubState( OPEN_AP ) )
         {
            if ( WiFi.status() != WL_DISCONNECTED )
            {
               WiFi.disconnect( true );
            }
            WiFi.mode( WIFI_AP );
            WiFi.softAP( "HAUSBUS-HOMESERVER" );
            DEBUG_M1( "AP IP address: " );
            Serial.print( WiFi.softAPIP() );
            SET_STATE_L2( RUN_AP );
         }
         else if ( inSubState( START_CONNECT ) )
         {
            if ( WiFi.status() != WL_DISCONNECTED )
            {
               WiFi.disconnect( true );
            }
            WiFi.mode( WIFI_STA );
            WiFi.begin( configuration->getSsidString(), configuration->getPasswordString() );
            SET_STATE_L2( WAIT_CONNECT );
         }
         else if ( inSubState( WAIT_CONNECT ) )
         {
            setSleepTime( 500 );
            if ( WiFi.status() == WL_CONNECTED )
            {
               SET_STATE_L2( CONNECTED );
               if ( !mdns.begin( String( "ESP-HBC-" + String( HACF::deviceId ) ).begin(), WiFi.localIP() ) )
               {
                  ERROR_1( "MDNS failed to start" );
               }
               DEBUG_M2( "myIP is ", WiFi.localIP().toString().begin() );
               new Gateway( new UdpStream(), Gateway::UDP_9 );
               new HomeAutomation();
            }
            if ( ++connectTries > MAX_CONNECT_TRIES )
            {
               ERROR_1( "Connect to WLAN failed!" )
               SET_STATE_L2( OPEN_AP );
            }
         }
         else if ( inSubState( CONNECTED ) )
         {

         }
         server.handleClient();
      }
      else if ( inIdle() )
      {

      }
   }
   else if ( event.isEvMessage() )
   {
      return handleRequest( event.isEvMessage()->getMessage() );
   }

   return consumed;
}

void HausBusWebServer::wakeUpDevice( const MAC& mac )
{
   // todo
}

bool HausBusWebServer::handleRequest( HACF* message )
{
   bool consumed = true;
   HACF::ControlFrame& cf = message->controlFrame;
   Command* data = reinterpret_cast<Command*>( cf.getData() );
   if ( cf.isCommand( Command::GET_CONFIGURATION ) )
   {
      DEBUG_H1( FSTR( ".getConfiguration()" ) );
      Response response( getId(), *message );
      configuration->get( response.setConfiguration().configuration );
      response.queue( getObject( message->header.getSenderId() ) );
   }
   else if ( cf.isCommand( Command::SET_CONFIGURATION ) )
   {
      DEBUG_H1( FSTR( ".setConfiguration()" ) );
      configuration->set( data->parameter.setConfiguration );
   }
   else if ( cf.isCommand( Command::WAKE_UP_DEVICE ) )
   {
      DEBUG_H1( FSTR( ".wakeUpDevice()" ) );
      wakeUpDevice( data->parameter.mac );
   }
   else if ( cf.isCommand( Command::GET_CURRENT_IP ) )
   {
      DEBUG_H1( FSTR( ".getCurrentIp()" ) );
      Response response( getId(), *message );
      response.setCurrentIp();
      response.queue( getObject( message->header.getSenderId() ) );
   }
   else
   {
      return false;
   }

   return consumed;
}

HausBusWebServer::Response::Parameter& HausBusWebServer::Response::setConfiguration()
{
   controlFrame.setDataLength( sizeof( getResponse() ) + sizeof( getParameter().configuration ) );
   setResponse( CONFIGURATION );
   return getParameter();
}

void HausBusWebServer::Response::setCurrentIp()
{
   controlFrame.setDataLength( sizeof( getResponse() ) + sizeof( getParameter().ip ) );
   setResponse( CURRENT_IP );
   getParameter().ip = WiFi.localIP();
}

void HausBusWebServer::onRoot()
{
   if ( inSubState( RUN_AP ) )
   {
      server.send( 200, "text/html", "<h1>You are connected</h1><br><br><a href='/config'>Click here to configure your network</a>" );
   }
   else
   {
      server.send( 200, "text/html", "<h1>Haus-Bus Configuration Web Server</h1>" );
   }
}

void HausBusWebServer::onSubmit()
{
   if ( server.args() > 1 && server.argName( 0 ) == "ssid" && server.argName( 1 ) == "pass" && server.argName( 2 ) == "boardRevision" )
   {
      DEBUG_M2( "new ssid = ", server.arg( 0 ).begin() );
      DEBUG_M2( "new pass = ", server.arg( 1 ).begin() );
      if ( server.arg( 0 ).length() <= Configuration::MAX_STRING_LEN )
      {
         if ( server.arg( 1 ).length() <= Configuration::MAX_STRING_LEN )
         {
            configuration->setSsidString( server.arg( 0 ).begin() );
            configuration->setPasswordString( server.arg( 1 ).begin() );
            server.send( 200, "text/html", "OK.<br>Reconnecting device with new network config......" );
            SET_STATE_L2( START_CONNECT );
            setSleepTime( 500 );
            return;
         }
      }
   }
   server.send( 200, "text/html", "<h1>something went wrong, please try again</h1><br><br><a href='/config'>Click here to configure your network</a>" );
}

void HausBusWebServer::onUpdate()
{
   if ( server.args() > 1 && server.argName( 0 ) == "ip" && server.argName( 1 ) == "file" )
   {
      Serial.print( "got firmware update command from ip = " );
      Serial.println( server.arg( 0 ) );
      Serial.print( ", file = " );
      Serial.println( server.arg( 1 ) );

      char logString[130];
      if ( millis() < 60000 )
      {
         sprintf( logString, "Busy... Please try again in %04d seconds.", ( 60000 - millis() ) / (uint)1000 );
         server.send( 200, "text/html", logString );
      }
      else
      {
/*
        ESPhttpUpdate.rebootOnUpdate(false);
        t_httpUpdate_return ret = ESPhttpUpdate.update(server.arg(0), 80, "/" + server.arg(1));
        char logString[130];
        if (ret == HTTP_UPDATE_FAILED) sprintf(logString, "Update failed %04d,%s,", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        else if (ret == HTTP_UPDATE_NO_UPDATES) sprintf(logString, "No Updates Pending");
        else if (ret == HTTP_UPDATE_OK) sprintf(logString, "Firmware update successfully finished. Booting device .....");
        else sprintf(logString, "unknown error");

        server.send(200, "text/html", logString);
        Serial.print("update returned ");
        Serial.println(logString);

        if (ret == HTTP_UPDATE_OK)
        {
          server.stop();
          server.close();
          delay(500);
          ESP.reset();
        }
 */
      }
   }
   else
   {
      server.send( 200, "text/html", "wrong params" );
   }
}

void HausBusWebServer::onConfig()
{
   server.send( 200, "text/html", "<h1>Network configuration</h1><p><form action='/submit' method='POST'><table border=0><tr><td>SSID:</td><td><input type=text name=ssid size=20></td></tr><tr><td>Password:</td><td><input type=text name=pass size=20></td></tr><tr><td>Board-ID:</td><td><input type=text name=boardRevision size=20></td></tr><td></td><td><input type='submit' value='Konfigurieren'></td></tr></table></form><br><br><a href='/restoreFactory'>Click here to resore factory configuration</a>" );
}

void HausBusWebServer::onRestorFactory()
{
   SPIFFS.format();
   server.send( 200, "text/html", "OK.<br>Rebooting device with AP HAUSBUS-HOMESERVER......" );
   delay( 500 );
   ESP.reset();
}