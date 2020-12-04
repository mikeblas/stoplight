
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

         if (now->tm_sec == 0 && lastBeepMinute != now->tm_sec)
         {
            buzzer.FourBeeps();
            lastBeepMinute = now->tm_sec;
            redTimeMilliseconds = 2000;
            greenTimeMilliseconds = 2000;
            yellowTimeMilliseconds = 2000;
            lights.SetRed(1);
            lights.SetGreen(1);
            lights.SetYellow(1);
         }

         if (now->tm_sec == 15 && lastBeepMinute != now->tm_sec)
         {
            buzzer.OneBeep();
            lastBeepMinute = now->tm_sec;
            redTimeMilliseconds = 1000;
            lights.SetRed(1);
         }

         if (now->tm_sec == 30 && lastBeepMinute != now->tm_sec)
         {
            buzzer.TwoBeeps();
            lastBeepMinute = now->tm_sec;
            redTimeMilliseconds = 1000;
            greenTimeMilliseconds = 1000;
            lights.SetRed(1);
            lights.SetGreen(1);
         }

         if (now->tm_sec == 45 && lastBeepMinute != now->tm_sec)
         {
            buzzer.ThreeBeeps();
            lastBeepMinute = now->tm_sec;
            redTimeMilliseconds = 1000;
            yellowTimeMilliseconds = 1000;
            lights.SetRed(1);
            lights.SetYellow(1);
         }

      }
   }

}

