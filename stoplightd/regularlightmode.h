
#pragma once

#include <condition_variable>
#include <mutex>

#include "modeinterface.h"

class RegularLightMode : public ModeInterface
{
   std::condition_variable cond;
   std::mutex mtx;

public:
   RegularLightMode(Lights& lights, daemonize::logger& log)
      : ModeInterface(lights, log)
   {
   }

   RegularLightMode(const RegularLightMode&) = delete;

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
      std::lock_guard<std::mutex> lk(mtx);
      cond.notify_one();
      quitting = true;
      return QUIT_MODE;
   }

   virtual void operator()() override;
};



