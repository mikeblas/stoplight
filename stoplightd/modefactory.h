#pragma once

#include "regularlightmode.h"
#include "selectormode.h"
#include "remomode.h"
#include "flickermode.h"
#include "clockmode.h"

class ModeFactory
{
public:
   static ModeInterface* FromModeNumber(int n, Lights& lights, daemonize::logger& log)
   {
      ModeInterface* p = nullptr;

      switch (n)
      {
         case 0:
            p = new SelectorMode(lights, log);
            break;

         case 1:
            p = new RegularLightMode(lights, log);
            break;

         case 2:
            p = new RemoMode(lights, log);
            break;

         case 3:
            p = new FlickerMode(lights, log);
            break;

         case 4:
            p = new ClockMode(lights, log);
            break;

         default:
            log << log.critical << "Invalid mode number: " << n << std::endl;
      }

      return p;
   }

};

