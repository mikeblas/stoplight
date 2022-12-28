#pragma once

#include "regularlightmode.h"
#include "selectormode.h"
#include "remomode.h"
#include "flickermode.h"
#include "clockmode.h"

#include "modeinterface.h"

class ModeFactory
{
public:
   static ModeInterface* FromModeNumber(ModeID n, Lights& lights, SmartBuzzer& buzzer, daemonize::logger& log)
   {
      ModeInterface* p = nullptr;

      switch (n)
      {
         case SELECTOR_MODE:
            p = new SelectorMode(lights, log);
            break;

         case REGULAR_MODE:
            p = new RegularLightMode(lights, log);
            break;

         case REMO_MODE:
            p = new RemoMode(lights, log);
            break;

         case FLICKER_MODE:
            p = new FlickerMode(lights, log);
            break;

         case CLOCK_MODE:
            p = new ClockMode(lights, buzzer, log);
            break;

         default:
            log << log.critical << "Invalid mode number: " << n << std::endl;
      }

      return p;
   }

};

