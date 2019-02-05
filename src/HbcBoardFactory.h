/*
 * HbcBoardFactory.h
 *
 *  Created on: 18.01.2019
 *      Author: Viktor Pankraz
 */

#ifndef HbcBoardFactory_H
#define HbcBoardFactory_H

#include <HomeAutomation.h>
#include <DigitalPort.h>

class HbcBoardFactory
{
   public:

      static HomeAutomation* createBoard();

      class HBC_Generic : public HomeAutomation
      {
         public:
            HBC_Generic();

         protected:
      };

      class HBC_IO8_1W : public HBC_Generic
      {
         public:
            HBC_IO8_1W();

         protected:
            DigitalPort digitalPort;
      };

   protected:

   private:

};

#endif