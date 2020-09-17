



#pragma once

#include <daemonize/daemonizer.hpp>
#include <daemonize/syslog.hpp>

#include <gpiod.h>

#include "modeinterface.h"

#include "buttons.h"
#include "lights.h"
#include "buzzer.h"


class stoplightd : public daemonize::daemon
{

   Lights* lights;
   Buttons* buttons;
   Buzzer* buzzer;

   int buzzticks;
   struct gpiod_chip* chip;

public:
   stoplightd(daemonize::logger& log);

   ~stoplightd()
   {
      delete lights;
      delete buttons;
      delete buzzer;
   }

   virtual void run() override;

private:
   void dispatchmessages(ModeInterface* mode, Buttons& b2);
};




