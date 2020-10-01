
#pragma once

#include <condition_variable>
#include <mutex>

#include "modeinterface.h"

class RegularLightMode : public ModeInterface
{
   std::condition_variable cond;
   std::mutex mtx;

   int redTimeMilliseconds;
   int yellowTimeMilliseconds;
   int greenTimeMilliseconds;
   int multiplier;

   bool reloop;

public:
   RegularLightMode(Lights& lights, daemonize::logger& log)
      : ModeInterface(lights, log),
        multiplier(5)
   {
      SetFromMultiplier();
   }

   RegularLightMode(const RegularLightMode&) = delete;

   void SetFromMultiplier();

   virtual void Shutdown() override
   {
      quitting = true;
      lights.AllOff();
   }

   virtual ButtonResult OnDButtonPressed() override
   {
      log << log.critical << "RegularLightMode: DButtonPressed" << std::endl;
      return NOTHING;
   }

   virtual ButtonResult OnDButtonReleased() override
   {
      log << log.critical << "RegularLightMode: DButtonReleased" << std::endl;

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

      log << log.critical << "RegularLightMode: Multipler changed to " << multiplier << std::endl;
      SetFromMultiplier();

      return NOTHING;
   }

   virtual void operator()() override;
};



