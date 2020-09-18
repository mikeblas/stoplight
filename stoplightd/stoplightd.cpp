#include <unistd.h>


#include <thread>
#include <chrono>
#include <condition_variable>
#include <mutex>

#include <daemonize/daemonizer.hpp>
#include <daemonize/syslog.hpp>

#include "modeinterface.h"

#include "lights.h"
#include "buttons.h"
#include "buzzer.h"

#include "regularlightmode.h"
#include "selectormode.h"

#include "stoplightd.h"

static const char* STOPLIGHT_CLIENT = "stoplight_remo";

class ModeFactory
{
public:

};


stoplightd::stoplightd(daemonize::logger& log)
   : daemon(log, "stoplightdpid", "/home/pi/"),
     lights(nullptr), buttons(nullptr), buzzer(nullptr),
     buzzticks(0), chip(nullptr)
{
   log << log.critical << "entered run function" << std::endl;

   log << log.critical << "gpiod version: " << gpiod_version_string() << std::endl;

   static const char* chipname = "gpiochip0";

   chip = gpiod_chip_open_by_name(chipname);
   if (chip == nullptr)
   {
      log << log.critical << "Open chip failed" << std::endl;
   }
   else
   {
      lights = new Lights(STOPLIGHT_CLIENT, chip);
      buttons = new Buttons(STOPLIGHT_CLIENT, chip);
      buzzer = new Buzzer(STOPLIGHT_CLIENT, chip);

      lights->AllOff();
   }
}


void stoplightd::dispatchmessages(ModeInterface* mode, Buttons& b2)
{
   // if this is a release decide what to do
   if (b2.GetVT() == 1)
   {
      buzzer->On();
      buzzticks = 1;

      if (b2.GetAButton())
      {
         mode->OnAButtonPressed();
      }
      if (b2.GetBButton())
      {
         mode->OnBButtonPressed();
      }
      if (b2.GetCButton())
      {
         mode->OnCButtonPressed();
      }
      if (b2.GetDButton())
      {
         mode->OnDButtonPressed();
      }
   }
   else
   {
      if (b2.GetAButton())
      {
         mode->OnAButtonReleased();
      }
      if (b2.GetBButton())
      {
         mode->OnBButtonReleased();
      }
      if (b2.GetCButton())
      {
         mode->OnCButtonReleased();
      }
      if (b2.GetDButton())
      {
         mode->OnDButtonReleased();
      }
   }
}




void stoplightd::run()
{
   lights->AllOff();

   // RegularLightMode mode(lights, log);
   SelectorMode mode(*lights, log);
   std::thread my_thread(std::ref(mode));

   buttons->ReadLines();
   int lastVT = buttons->GetVT();
   int ignores = 0;

   while(1)
   {

      if (buzzticks > 0)
      {
         if (--buzzticks == 0)
         {
            buzzer->Off();
         }
      }

      if (ignores > 0)
      {
         ignores--;
      }
      else
      {

         buzzer->Off();

         Buttons b2(STOPLIGHT_CLIENT, chip);
         b2.ReadLines();

         // anything change since last time?
         if (b2 != *buttons || lastVT != b2.GetVT())
         {

            // yes! print out the flags
            buttons->LogState(log, "B1");
            b2.LogState(log, "B2");

            // ignore three loops to debounce
            ignores = 3;

            dispatchmessages(&mode, b2);

            // change states
            *buttons = b2;
            lastVT = b2.GetVT();
         }
      }

      usleep(100 * 1000);
      if (mode.IsQuitting())
      {
         log << log.critical << "Quitting!!" << std::endl;
         my_thread.join();
         break;
      }
   }
}


