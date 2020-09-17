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


static const char* STOPLIGHT_CLIENT = "stoplight_remo";

class RegularLightMode : public ModeInterface
{

   std::condition_variable cond;
   std::mutex mtx;

public:
   RegularLightMode(Lights* pLights, daemonize::logger& log)
      : ModeInterface(pLights, log)
      {
      }

   RegularLightMode(const RegularLightMode&) = delete;

   virtual void Shutdown() override
   {
      quitting = true;
   }

   virtual void OnDButtonPressed() override
   {
      log << log.critical << "RegularLightMode: DButtonPressed" << std::endl;
   }

   virtual void OnDButtonReleased() override
   {
      log << log.critical << "RegularLightMode: DButtonReleased" << std::endl;
      std::lock_guard<std::mutex> lk(mtx);
      cond.notify_one();
      quitting = true;
   }

   virtual void operator()() override
   {

      using namespace std::chrono_literals;

      int state = 0;

      while (!quitting)
      {

         log << log.critical << "quitting state is " << quitting << std::endl;

         std::unique_lock<std::mutex> lck(mtx);

         switch (state)
         {
            case 0:
               state = 1;
               pLights->AllOff();
               pLights->SetRed(1);
               log << log.critical << "Switched to red" << std::endl;
               // std::this_thread::sleep_for(24s);
               if (cond.wait_for(lck, 4s) != std::cv_status::timeout)
               {
                  quitting = true;
                  log << log.critical << "Signalled!!" << std::endl;
               }
               break;

            case 2:
               state = 0;
               pLights->AllOff();
               pLights->SetYellow(1);
               log << log.critical << "Switched to yellow" << std::endl;
               // std::this_thread::sleep_for(8s);
               if (cond.wait_for(lck, 2s) != std::cv_status::timeout)
               {
                  quitting = true;
                  log << log.critical << "Signalled!!" << std::endl;
               }
               break;

            case 1:
               state = 2;
               pLights->AllOff();
               pLights->SetGreen(1);
               log << log.critical << "Switched to green" << std::endl;
               // std::this_thread::sleep_for(88s);
               if (cond.wait_for(lck, 8s) != std::cv_status::timeout)
               {
                  quitting = true;
                  log << log.critical << "Signalled!!" << std::endl;
               }
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
         std::thread my_thread(std::ref(mode));

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
                  log << log.critical << "B2 = " << b2.GetAButton() << ", " <<  b2.GetBButton() << ", " << b2.GetCButton() << ", " << b2.GetDButton() << ", " << b2.GetVT() << std::endl;

                  // ignore three loops to debounce
                  ignores = 3;

                  // if this is a release decide what to do
                  if (b2.GetVT() == 1)
                  {

                     buzz.On();
                     buzzticks = 1;

                     if (b2.GetAButton())
                     {
                        mode.OnAButtonPressed();
                     }
                     if (b2.GetBButton())
                     {
                        mode.OnBButtonPressed();
                     }
                     if (b2.GetCButton())
                     {
                        mode.OnCButtonPressed();
                     }
                     if (b2.GetDButton())
                     {
                        mode.OnDButtonPressed();
                     }
                  }
                  else
                  {
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
            if (mode.IsQuitting())
            {
               log << log.critical << "Quitting!!" << std::endl;
               my_thread.join();
               break;
            }
         }
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

