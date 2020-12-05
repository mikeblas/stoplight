
#include "clockmode.h"
#include <ctime>


// this helps compare either seconds or minutes passing;
// running seconds helps with impatient debugging,
// minutes is for deployment
class ClockComparitor
{
   bool minutes_mode;
   int lastComparisonUnit;

public:
   ClockComparitor(bool mode) 
   :  minutes_mode{mode}, lastComparisonUnit{-1}
   {
   }


   bool Compare(int offset, std::tm* tm)
   {
      int* p = minutes_mode ? &(tm->tm_min) : &(tm->tm_sec);

      if (*p == offset && lastComparisonUnit != *p)
      {
         lastComparisonUnit = *p;
         return true;
      }

      return false;
   }


};


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
   ClockComparitor comparitor(false);

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

         // at the top, we flip everything and beep four
         if (comparitor.Compare(0, now))
         {
            buzzer.FourBeeps();
            redTimeMilliseconds = 2000;
            greenTimeMilliseconds = 2000;
            yellowTimeMilliseconds = 2000;
            lights.SetRed(1);
            lights.SetGreen(1);
            lights.SetYellow(1);
         }

         // at 15, we flip only red with just one beep
         if (comparitor.Compare(15, now))
         {
            buzzer.OneBeep();
            redTimeMilliseconds = 1000;
            lights.SetRed(1);
         }

         // at 30, we flip yellow with two beeps
         if (comparitor.Compare(30, now))
         {
            buzzer.TwoBeeps();
            yellowTimeMilliseconds = 1000;
            lights.SetYellow(1);
         }

         // at 45, we flip green with three beeps
         if (comparitor.Compare(45, now))
         {
            buzzer.ThreeBeeps();
            greenTimeMilliseconds = 1000;
            lights.SetGreen(1);
         }

      }
   }

}

