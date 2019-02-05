/*
 * HbcBoardFactory.cpp
 *
 *  Created on: 18.01.2019
 *      Author: Viktor Pankraz
 */


#include "HbcBoardFactory.h"
#include "UdpStream.h"
#include <DS1820.h>
#include <Electronics.h>


HomeAutomation* HbcBoardFactory::createBoard()
{
   switch ( HomeAutomationHw::getFckE() )
   {
      case ESP_IO8_1W:
      {
         return new HBC_IO8_1W();
      }
   }
   return new HBC_Generic();
}

HbcBoardFactory::HBC_Generic::HBC_Generic()
{

}

HbcBoardFactory::HBC_IO8_1W::HBC_IO8_1W() : digitalPort( 0 )
{
   DS1820::scanAndCreateDevices( PortPin( D4 ) );
   digitalPort.setPins( 0, PortPin( D0 ) );
   digitalPort.setPins( 1, PortPin( D1 ) );
   digitalPort.setPins( 2, PortPin( D2 ) );
   digitalPort.setPins( 3, PortPin( D3 ) );
   digitalPort.setPins( 4, PortPin( D5 ) );
   digitalPort.setPins( 5, PortPin( D6 ) );
   digitalPort.setPins( 6, PortPin( D7 ) );
   digitalPort.setPins( 7, PortPin( D8 ) );
}