
#include <chrono>

#include "regularlightmode.h"


void RegularLightMode::operator()()
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
            lights.AllOff();
            lights.SetRed(1);
            log << log.critical << "Switched to red" << std::endl;
            if (cond.wait_for(lck, 4s) != std::cv_status::timeout)
            {
               quitting = true;
               log << log.critical << "Signalled!!" << std::endl;
            }
            break;

         case 2:
            state = 0;
            lights.AllOff();
            lights.SetYellow(1);
            log << log.critical << "Switched to yellow" << std::endl;
            if (cond.wait_for(lck, 2s) != std::cv_status::timeout)
            {
               quitting = true;
               log << log.critical << "Signalled!!" << std::endl;
            }
            break;

         case 1:
            state = 2;
            lights.AllOff();
            lights.SetGreen(1);
            log << log.critical << "Switched to green" << std::endl;
            if (cond.wait_for(lck, 8s) != std::cv_status::timeout)
            {
               quitting = true;
               log << log.critical << "Signalled!!" << std::endl;
            }
            break;
      }
   }
}


