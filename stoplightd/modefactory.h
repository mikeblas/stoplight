#pragma once

#include "regularlightmode.h"
#include "selectormode.h"
#include "remomode.h"

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

         default:
            log << log.critical << "Invalid mode number: " << n << std::endl;
      }

      return p;
   }

};

