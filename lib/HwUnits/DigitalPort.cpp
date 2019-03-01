/*
 * DigitalPort.cpp
 *
 *  Created on: 28.08.2014
 *      Author: Viktor Pankraz
 */

#include "DigitalPort.h"
#include "DigitalOutputUnit.h"
#include "Button.h"
#include "DS1820.h"

#include <ErrorMessage.h>

const uint8_t DigitalPort::debugLevel( DEBUG_LEVEL_OFF );

DigitalPort::Response::Parameter& DigitalPort::Response::setConfiguration()
{
   controlFrame.setDataLength( sizeof( getResponse() ) + sizeof( getParameter().configuration ) );
   setResponse( CONFIGURATION );
   return getParameter();
}

bool DigitalPort::notifyEvent( const Event& event )
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

void DigitalPort::run()
{
   if ( inStartUp() )
   {
      setConfiguration( ConfigurationManager::getConfiguration<EepromConfiguration>( id ) );
      if ( configuration )
      {
         configureHw();
         SET_STATE_L1( RUNNING );
      }
      else
      {
         terminate();
         ErrorMessage::notifyOutOfMemory( id );
         return;
      }
   }
   else
   {
      setSleepTime( NO_WAKE_UP );
   }

}

bool DigitalPort::handleRequest( HACF* message )
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
   else
   {
      return false;
   }

   return consumed;
}

void DigitalPort::configureHw()
{
   uint8_t i = Configuration::MAX_PINS;
   uint8_t bit = 0x80;
   while ( i-- )
   {
      bool notSupported = false;
      uint8_t pinFunction = configuration->getPinFunction( i );
      if ( pinFunction != 0xFF )
      {
         if ( isPinUsable( bit ) )
         {
            if ( pinFunction == ClassId::BUTTON )
            {
               new Button( pins[i] );
            }
            else if ( pinFunction == ClassId::DIGITAL_OUTPUT )
            {
               new DigitalOutputUnit( pins[i] );
            }
            else
            {
               notSupported = true;
            }
         }
         else
         {
            notSupported = true;
         }
      }
      if ( notSupported )
      {
         Response event( getId() );
         event.setErrorCode( PINFUNCTION_NOT_SUPPORTED + i );
         event.queue();
         configuration->setPinFunction( i, 0xFF );
      }
      bit >>= 1;
   }
}