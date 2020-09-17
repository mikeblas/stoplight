
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
   }

   virtual void OnDButtonPressed() override
   {
      log << log.critical << "RegularLightMode: DButtonPressed" << std::endl;
   }

   virtual void OnDButtonReleased() override
   {
      log << log.critical << "RegularLightMode: DButtonReleased" << std::endl;
      std::lock_guard<std::mutex> lk(mtx);
      cond.notify_one();
      quitting = true;
   }

   virtual void operator()() override;
};



