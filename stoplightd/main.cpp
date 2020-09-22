
#include <unistd.h>

#include <daemonize/daemonizer.hpp>
#include <daemonize/syslog.hpp>
#include <daemonize/signals.hpp>

#include "stoplightd.h"

int main(int /* argc */, const char** /* argv */)
{

   daemonize::syslog syslog("stoplightd");
   daemonize::signals::enable(&syslog);

   try
   {
      stoplightd d(syslog);
      daemonize::daemonizer(d);
   }
   catch(int status)
   {
      syslog << syslog.critical << "exiting with status " << status << std::endl;
      return status;
   }
   catch(...)
   {

      syslog << syslog.critical << "exiting with exception!" << std::endl;
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}



