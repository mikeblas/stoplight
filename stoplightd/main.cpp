
#include <unistd.h>

#include <daemonize/daemonizer.hpp>
#include <daemonize/syslog.hpp>

#include "stoplightd.h"

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



