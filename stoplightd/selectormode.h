
#pragma once

#include <condition_variable>
#include <mutex>

#include "modeinterface.h"

class SelectorMode : public ModeInterface
{
   std::condition_variable cond;
   std::mutex mtx;

   int mode;
   int newMode;

public:
   SelectorMode(Lights& lights, daemonize::logger& log)
      : ModeInterface(lights, log),
        mode(0), newMode(-1)
   {
   }

   SelectorMode(const SelectorMode&) = delete;

   virtual void Shutdown() override
   {
      quitting = true;
      lights.AllOff();
   }

   virtual ButtonResult OnAButtonReleased() override
   {
      log << log.critical << "SelectorMode: AButtonReleased" << std::endl;
      if (mode < 7)
      {
          mode++;
          UpdateLights();
      }
      return NOTHING;
   }

   virtual ButtonResult OnBButtonReleased() override
   {
      log << log.critical << "SelectorMode: BButtonReleased" << std::endl;
      if (mode > 0)
      {
          mode--;
          UpdateLights();
      }
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

   virtual ButtonResult OnCButtonReleased() override
   {
      log << log.critical << "RegularLightMode: CButtonReleased" << std::endl;
      std::lock_guard<std::mutex> lk(mtx);
      cond.notify_one();
      newMode = mode;
      return CHANGE_MODE;
   }

   virtual int NewMode() override
   {
      return newMode;
   }

   virtual void operator()() override;

private:
   void UpdateLights();
};

