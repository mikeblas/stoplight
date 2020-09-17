#include <unistd.h>

#include <thread>
#include <chrono>

#include <daemonize/daemonizer.hpp>
#include <daemonize/syslog.hpp>

#include "lights.h"
#include "buttons.h"
#include "buzzer.h"


static const char* STOPLIGHT_CLIENT = "stoplight_remo";

class ModeInterface
{

protected:
   Lights* pLights;
   daemonize::logger& log;

public:
   ModeInterface(Lights* pLights, daemonize::logger& log)
      : pLights(pLights), log(log)
      {
      }

   virtual void operator()() = 0;
   virtual void Shutdown() = 0;

   virtual void OnAButtonPressed() { }
   virtual void OnAButtonReleased() { }
   virtual void OnBButtonPressed() { }
   virtual void OnBButtonReleased() { }
   virtual void OnCButtonPressed() { }
   virtual void OnCButtonReleased() { }
   virtual void OnDButtonPressed() { }
   virtual void OnDButtonReleased() { }
};

class RegularLightMode : public ModeInterface
{

   bool quitting = false;
public:
   RegularLightMode(Lights* pLights, daemonize::logger& log)
      : ModeInterface(pLights, log)
      {
      }

   virtual void Shutdown() override
   {
      quitting = true;
   }

   virtual void operator()() override
   {

      using namespace std::chrono_literals;

      int state = 0;

      while (!quitting)
      {
         switch (state)
         {
            case 0:
               state = 1;
               pLights->AllOff();
               pLights->SetRed(1);
               log << log.critical << "Switched to red" << std::endl;
               std::this_thread::sleep_for(24s);
               break;

            case 2:
               state = 0;
               pLights->AllOff();
               pLights->SetYellow(1);
               log << log.critical << "Switched to yellow" << std::endl;
               std::this_thread::sleep_for(8s);
               break;

            case 1:
               state = 2;
               pLights->AllOff();
               pLights->SetGreen(1);
               log << log.critical << "Switched to green" << std::endl;
               std::this_thread::sleep_for(88s);
               break;
         }
      }
   }

   // we don't listen to any buttons, so no overrides
};

class ModeFactory
{
public:

};


class stoplightd : public daemonize::daemon
{

public:
   stoplightd(daemonize::logger& log)
   : daemon(log, "stoplightdpid", "/home/pi/")
   {
   }

   virtual void run() {

      log << log.critical << "entered run function" << std::endl;

      log << log.critical << "gpiod version: " << gpiod_version_string() << std::endl;

      static const char* chipname = "gpiochip0";

      struct gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
      if (chip == nullptr)
      {
         log << log.critical << "Open chip failed" << std::endl;
      }
      else
      {

         Lights lights(STOPLIGHT_CLIENT, chip);
         Buttons buttons(STOPLIGHT_CLIENT, chip);
         Buzzer buzz(STOPLIGHT_CLIENT, chip);

         lights.AllOff();

         RegularLightMode mode(&lights, log);
         std::thread my_thread(mode);

         buttons.ReadLines();
         int lastVT = buttons.GetVT();
         int ignores = 0;
         int buzzticks = 0;

         while(1)
         {

            if (buzzticks > 0)
            {
               if (--buzzticks == 0)
               {
                  buzz.Off();
               }
            }

            if (ignores > 0)
            {
               ignores--;
            }
            else
            {

               buzz.Off();

               Buttons b2(STOPLIGHT_CLIENT, chip);
               b2.ReadLines();

               // anything change since last time?
               if (b2 != buttons || lastVT != b2.GetVT())
               {

                  // yes! print out the flags
                  log << log.critical << "B1 = " << buttons.GetAButton() << ", " <<  buttons.GetBButton() << ", " << buttons.GetCButton() << ", " << buttons.GetDButton() << ", " << buttons.GetVT() << std::endl;
//                  printf("b1 = %d, %d, %d, %d, %d\n", b1.GetAButton(), b1.GetBButton(), b1.GetCButton(), b1.GetDButton(), b1.GetVT());
                  printf("b2 = %d, %d, %d, %d, %d\n", b2.GetAButton(), b2.GetBButton(), b2.GetCButton(), b2.GetDButton(), b2.GetVT());

                  // ignore three loops to debounce
                  ignores = 3;

                  // if this is a release decide what to do
                  if (b2.GetVT() == 1)
                  {

                     buzz.On();
                     buzzticks = 1;

                     printf("***\n");
                     if (b2.GetAButton())
                     {
                        mode.OnAButtonReleased();
                     }
                     if (b2.GetBButton())
                     {
                        mode.OnBButtonReleased();
                     }
                     if (b2.GetCButton())
                     {
                        mode.OnCButtonReleased();
                     }
                     if (b2.GetDButton())
                     {
                        mode.OnDButtonReleased();
                     }
                  }

                  // change states
                  buttons = b2;
                  lastVT = b2.GetVT();
               }
            }

            usleep(100 * 1000);
         }

         my_thread.join();

      }
   }
};

int main(int /* argc */, const char** /* argv */) {

   try
   {
      daemonize::syslog syslog("stoplightd");
      stoplightd d(syslog);
      daemonize::daemonizer(d);
   }
   catch(int status)
   {
      return status;
   }
   catch(...)
   {
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}

