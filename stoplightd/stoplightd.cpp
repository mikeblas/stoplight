#include <unistd.h>


#include <thread>
#include <chrono>

#include <daemonize/daemonizer.hpp>
#include <daemonize/syslog.hpp>

#include "modeinterface.h"

#include "modefactory.h"

#include "lights.h"
#include "buttons.h"
#include "buzzer.h"

#include "regularlightmode.h"
#include "selectormode.h"

#include "stoplightd.h"

static const char* STOPLIGHT_CLIENT = "stoplight_remo";



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
   int ignores = 0;

   lights->AllOff();

   // start up with null mode, which is standby/dark
   ModeInterface* mode = nullptr;
   std::thread* theThread = nullptr;

   bool selectorMode = false;

   // get the lines, and get started!
   buttons->ReadLines();
   int lastVT = buttons->GetVT();

   // loop until we quit ...
   while (true)
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

         // if we have no mode, just see if any button pressed
         if (mode == nullptr)
         {
            if (b2.IsAnyButtonDown())
            {
               // enter selector mode
               mode = ModeFactory::FromModeNumber(0, *lights, log);
               selectorMode = true;

               log << log.critical << "starting selector mode thread" << std::endl;
               theThread = new std::thread(std::ref(*mode));
            }
         }
         else
         {
            // anything change since last time?
            if (b2 != *buttons || lastVT != b2.GetVT())
            {

               // yes! print out the flags
               buttons->LogState(log, "B1");
               b2.LogState(log, "B2");

               // ignore three loops to debounce
               ignores = 3;

               dispatchmessages(mode, b2);

               // change states
               *buttons = b2;
               lastVT = b2.GetVT();
            }

            // did we quit?
            if (mode->IsQuitting())
            {
               log << log.critical << "Quitting! waiting for mode thread ..." << std::endl;
               theThread->join();
               delete theThread;
               log << log.critical << "mode thread joined" << std::endl;

               // delete the mode
               delete mode;
               mode = nullptr;

               // if we were in selector mode, go to sleep
               if (selectorMode)
               {
                  selectorMode = false;
                  log << log.critical << "returning to idle mode" << std::endl;
               }
               else
               {
                  // otherwise, it was a work mode and we return to selector mode
                  log << log.critical << "returning to selector mode" << std::endl;
               }
            }

            // are we switching modes?
            int newMode = mode->NewMode();
            if (newMode != -1)
            {
               log << log.critical << "New mode selected: " << newMode << std::endl;
               mode->Shutdown();
               mode = ModeFactory::FromModeNumber(newMode + 1, *lights, log);

               log << log.critical << "starting new mode thread" << std::endl;
               theThread = new std::thread(std::ref(*mode));
            }
         }
      }

      // sleep 100 ms between tests
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(100ms);
   }
}


