
#pragma once

#include <condition_variable>
#include <mutex>
#include <random>

#include "modeinterface.h"

class FlickerMode : public ModeInterface
{
   std::condition_variable cond;
   std::mutex mtx;

   std::default_random_engine generator;
   std::uniform_int_distribution<int>* dist;

   int redTimeMilliseconds;
   int yellowTimeMilliseconds;
   int greenTimeMilliseconds;
   int multiplier;

   int minimumMillis;
   int maximumMillis;

   bool reloop;

   void WorkLight(Lights::LightLine light, int* millisRemaining);

public:
   FlickerMode(Lights& lights, daemonize::logger& log)
      : ModeInterface(lights, log),
        dist(nullptr),
        multiplier(5)
   {
      SetFromMultiplier();

      redTimeMilliseconds = dist->operator()(generator);
      greenTimeMilliseconds = dist->operator()(generator);
      yellowTimeMilliseconds = dist->operator()(generator);
   }

   FlickerMode(const FlickerMode&) = delete;

   void SetFromMultiplier();

   ~FlickerMode()
   {
      delete dist;
   }

   virtual void Shutdown() override
   {
      quitting = true;
      lights.AllOff();
   }

   virtual ButtonResult OnDButtonPressed() override
   {
      log << log.critical << "FlickerMode: DButtonPressed" << std::endl;
      return NOTHING;
   }

   virtual ButtonResult OnDButtonReleased() override
   {
      log << log.critical << "FlickerMode: DButtonReleased" << std::endl;

      // set the lock and indicate quitting
      std::lock_guard<std::mutex> lk(mtx);
      cond.notify_one();
      quitting = true;

      return QUIT_MODE;
   }

   virtual ButtonResult OnCButtonReleased() override
   {
      if (++multiplier == 11)
         multiplier = 1;

      log << log.critical << "FlickerMode: Multipler changed to " << multiplier << std::endl;
      SetFromMultiplier();

      return NOTHING;
   }

   virtual void operator()() override;
};



