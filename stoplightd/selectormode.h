
#pragma once

#include <condition_variable>
#include <mutex>

#include "modeinterface.h"

class SelectorMode : public ModeInterface
{
   std::condition_variable cond;
   std::mutex mtx;

   int mode;

public:
   SelectorMode(Lights& lights, daemonize::logger& log)
      : ModeInterface(lights, log),
        mode(0)
   {
   }

   SelectorMode(const SelectorMode&) = delete;

   virtual void Shutdown() override
   {
      quitting = true;
   }

   virtual void OnAButtonReleased() override
   {
      log << log.critical << "SelectorMode: AButtonReleased" << std::endl;
      if (mode < 7)
      {
          mode++;
          UpdateLights();
      }
   }

   virtual void OnBButtonReleased() override
   {
      log << log.critical << "SelectorMode: BButtonReleased" << std::endl;
      if (mode > 0)
      {
          mode--;
          UpdateLights();
      }
   }

   virtual void OnDButtonReleased() override
   {
      log << log.critical << "RegularLightMode: DButtonReleased" << std::endl;
      std::lock_guard<std::mutex> lk(mtx);
      cond.notify_one();
      quitting = true;
   }

   virtual void OnCButtonReleased() override
   {
      log << log.critical << "RegularLightMode: CButtonReleased" << std::endl;
      std::lock_guard<std::mutex> lk(mtx);
      cond.notify_one();
   }

   virtual void operator()() override;

private:
   void UpdateLights();
};

