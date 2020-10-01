
#include <chrono>
#include <random>

#include "flickermode.h"


void FlickerMode::operator()()
{

   using namespace std::chrono_literals;

   std::unique_lock<std::mutex> lck(mtx);

   while (!quitting)
   {
      WorkLight(Lights::LIGHT_RED, &redTimeMilliseconds);
      WorkLight(Lights::LIGHT_YELLOW, &yellowTimeMilliseconds);
      WorkLight(Lights::LIGHT_GREEN, &greenTimeMilliseconds);

      if (cond.wait_for(lck, std::chrono::milliseconds(100)) != std::cv_status::timeout)
      {
         log << log.critical << "Signalled!" << std::endl;
      }
   }
}


void FlickerMode::WorkLight(Lights::LightLine light, int* millisRemaining)
{
   if (*millisRemaining <= 0)
   {
      *millisRemaining = dist->operator()(generator);
      lights.Toggle(light);
   }
   else
   {
      *millisRemaining -= 100;
   }
}


void FlickerMode::SetFromMultiplier()
{

   minimumMillis = 100 * multiplier;
   maximumMillis = 200 * (multiplier * 3);

   if (dist != nullptr)
      delete dist;

   dist = new std::uniform_int_distribution<int>(minimumMillis, maximumMillis);
}

