/*
 * ResetSystem.h
 *
 *  Created on: 17.07.2017
 *      Author: Viktor Pankraz
 */

#ifndef ResetSystem_H
#define ResetSystem_H

#include <Arduino.h>

class ResetSystem
{
   ////    Operations    ////

   public:

      ///*! \brief This function lock the entire clock system configuration.
      // *
      // *  This will lock the configuration until the next reset, or until the
      // *  External Oscillator Failure Detections (XOSCFD) feature detects a failure
      // *  and switches to internal 2MHz RC oscillator.
      // */

      inline static void clearSources( uint8_t sources = 0xFF )
      {

      }

      ///*! \brief This function lock the entire clock system configuration.
      // *
      // *  This will lock the configuration until the next reset, or until the
      // *  External Oscillator Failure Detections (XOSCFD) feature detects a failure
      // *  and switches to internal 2MHz RC oscillator.
      // */
      inline static uint8_t getSources()
      {
         return 0;
      }

      inline static uint8_t isBrownOutReset()
      {
         return false;
      }

      inline static uint8_t isDebuggerReset()
      {
         return false;
      }

      inline static uint8_t isExternalReset()
      {
         return false;
      }

      inline static uint8_t isPowerOnReset()
      {
         return false;
      }

      inline static uint8_t isSoftwareReset()
      {
         return false;
      }

      inline static uint8_t isSpikeDetectorReset()
      {
         return false;
      }

      inline static uint8_t isWatchDogReset()
      {
         return false;
      }

      ///*! \brief This function lock the entire clock system configuration.
      // *
      // *  This will lock the configuration until the next reset, or until the
      // *  External Oscillator Failure Detections (XOSCFD) feature detects a failure
      // *  and switches to internal 2MHz RC oscillator.
      // */
      inline static void reset( bool hard = false )
      {
         hard ? ESP.reset() : ESP.restart();
      }
};

#endif
