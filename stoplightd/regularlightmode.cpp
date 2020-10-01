
#include <chrono>

#include "regularlightmode.h"


void RegularLightMode::operator()()
{

   using namespace std::chrono_literals;

   int state = 0;

   while (!quitting)
   {

      std::unique_lock<std::mutex> lck(mtx);
      reloop = false;

      switch (state)
      {
         case 0:
            state = 1;
            lights.AllOff();
            lights.SetRed(1);
            log << log.critical << "Switched to red" << std::endl;
            if (cond.wait_for(lck, std::chrono::milliseconds(redTimeMilliseconds)) != std::cv_status::timeout)
            {
               log << log.critical << "Signalled!!" << std::endl;
            }
            break;

         case 2:
            state = 0;
            lights.AllOff();
            lights.SetYellow(1);
            log << log.critical << "Switched to yellow" << std::endl;
            if (cond.wait_for(lck, std::chrono::milliseconds(yellowTimeMilliseconds)) != std::cv_status::timeout)
            {
               log << log.critical << "Signalled!!" << std::endl;
            }
            break;

         case 1:
            state = 2;
            lights.AllOff();
            lights.SetGreen(1);
            log << log.critical << "Switched to green" << std::endl;
            if (cond.wait_for(lck, std::chrono::milliseconds(greenTimeMilliseconds)) != std::cv_status::timeout)
            {
               log << log.critical << "Signalled!!" << std::endl;
            }
            break;
      }
   }
}

void RegularLightMode::SetFromMultiplier()
{

   redTimeMilliseconds = 2000 * multiplier;
   yellowTimeMilliseconds = 1000 * multiplier;
   greenTimeMilliseconds = 4000 * multiplier;

   // set the lock to reloop to new timing
   std::lock_guard<std::mutex> lk(mtx);
   cond.notify_one();

   reloop = true;
}

