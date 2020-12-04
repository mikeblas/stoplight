
#include "clockmode.h"
#include <ctime>


void ClockMode::WorkLight(Lights::LightLine light, int* millisRemaining)
{
   if (*millisRemaining <= 0)
   {
      lights.OffNum(light);
   }
   else
   {
      *millisRemaining -= 100;
   }
}

void ClockMode::operator()()
{
   log << log.critical << "ClockMode starting" << std::endl;
   lights.AllOff();

   using namespace std::chrono_literals;
   std::unique_lock<std::mutex> lck(mtx);

   int lastIndicatorSecond = -1;

   while (!quitting)
   {
      WorkLight(Lights::LIGHT_RED, &redTimeMilliseconds);
      WorkLight(Lights::LIGHT_YELLOW, &yellowTimeMilliseconds);
      WorkLight(Lights::LIGHT_GREEN, &greenTimeMilliseconds);

      if (cond.wait_for(lck, std::chrono::milliseconds(100)) != std::cv_status::timeout)
      {
         log << log.critical << "Signalled!" << std::endl;
      }
      else
      {
         std::time_t t = std::time(0);
         std::tm* now = std::localtime(&t);

         // indicator 1 toggles every second
         if (lastIndicatorSecond != now->tm_sec)
         {
            lastIndicatorSecond = now->tm_sec;
            lights.ToggleInd1();
         }

         if (now->tm_sec == 0 && lastBeepMinute != now->tm_min)
         {
//            buzzer->OneBeep();
            lastBeepMinute = now->tm_min;
            redTimeMilliseconds = 1000;
            lights.SetRed(1);
         }

      }
   }

}

