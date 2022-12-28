
#pragma once

#include <condition_variable>
#include <mutex>

#include "modeinterface.h"

class SelectorMode : public ModeInterface
{
   std::condition_variable cond;
   std::mutex mtx;

   ModeID mode;
   ModeID newMode;

public:
   SelectorMode(Lights& lights, daemonize::logger& log)
      : ModeInterface(lights, log),
        mode(SELECTOR_MODE), newMode(NO_MODE)
   {
   }

   SelectorMode(const SelectorMode&) = delete;

   virtual ModeID GetModeID() { return SELECTOR_MODE; }
   virtual std::string GetModeName() { return "Selector"; }

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
          mode = (ModeID) (mode + 1);
          UpdateLights();
      }
      return NOTHING;
   }

   virtual ButtonResult OnBButtonReleased() override
   {
      log << log.critical << "SelectorMode: BButtonReleased" << std::endl;
      if (mode > 0)
      {
          mode = (ModeID) (mode - 1);
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

   virtual ModeID NewMode() override
   {
      // the first displayed mode number is 0 here, so convert
      // from that to an actual ModeID because selector mode can't be selected
      if (newMode == NO_MODE)
          return NO_MODE;
      return (ModeID) (newMode + 1);
   }

   virtual void operator()() override;

private:
   void UpdateLights();
};

