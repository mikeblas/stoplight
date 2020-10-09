
#pragma once

#include <thread>

#include <daemonize/daemonizer.hpp>
#include <daemonize/syslog.hpp>

#include <gpiod.h>

#include "modeinterface.h"

#include "buttons.h"
#include "lights.h"
#include "smartbuzzer.h"


class stoplightd : public daemonize::daemon
{

   Lights* lights;
   Buttons* buttons;
   SmartBuzzer* buzzer;

   struct gpiod_chip* chip;

   ModeInterface* mode;
   bool selectorMode;
   bool needRelease;
   bool runner;
   std::thread* theThread;

   void SetupRunner();

public:
   stoplightd(daemonize::logger& log);

   ~stoplightd()
   {

      if (!runner)
      {
         log << log.notice << "stoplightd dtor: not the runner" << std::endl;
      }
      else
      {
      log << log.notice << "stoplightd dtor: runner!" << std::endl;

         if (lights == nullptr)
            log << log.notice << "lights is null" << std::endl;
         else
            log << log.notice << "lights is not null" << std::endl;

         delete lights;
         delete buttons;
         delete buzzer;
      }
   }

   virtual void run() override;

private:
   void DispatchMessages(ModeInterface* mode, Buttons& b2);

   void StartSelectorMode();
};



