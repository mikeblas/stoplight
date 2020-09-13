
#include <unistd.h>

#include <daemonize/daemonizer.hpp>
#include <daemonize/syslog.hpp>

#include "lights.h"

class my_daemon : public daemonize::daemon {

public:
   my_daemon(daemonize::logger& log)
   : daemon(log, "demon1pid", "/home/pi/")
   {
   }

   virtual void run() {

      log << log.critical << "entered run function" << std::endl;

      log << log.critical << "gpiod version: " << gpiod_version_string() << std::endl;

      static const char* chipname = "gpiochip0";

      struct gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
      if (chip == nullptr) {
         log << log.critical << "Open chip failed" << std::endl;
      }
      else {
         Lights lights(chip);

         lights.AllOff();

         int state = 0;

         while (true) {
            switch (state) {
               case 0: 
                  state = 1;
                  lights.AllOff();
                  lights.SetRed(1);
                  log << log.critical << "Switched to red" << std::endl;
                  usleep(1000 * 24 * 1000);
                  break;

               case 2:
                  state = 0;
                  lights.AllOff();
                  lights.SetYellow(1);
                  log << log.critical << "Switched to yellow" << std::endl;
                  usleep(1000 * 8 * 1000);
                  break;

               case 1:
                  state = 2;
                  lights.AllOff();
                  lights.SetGreen(1);
                  log << log.critical << "Switched to green" << std::endl;
                  usleep(1000 * 88 * 1000);
                  break;
            }
         }
      }
   }
};

int main(int /* argc */, const char** /* argv */) {

   try
   {
      daemonize::syslog syslog("my_daemon");
      my_daemon d(syslog);
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

