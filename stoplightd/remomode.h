#pragma once

#include <condition_variable>
#include <mutex>

#include "modeinterface.h"

class RemoMode : public ModeInterface
{
   std::condition_variable cond;
   std::mutex mtx;

public:
   RemoMode(Lights& lights, daemonize::logger& log)
      : ModeInterface(lights, log)
   {
   }

   RemoMode(const RemoMode&) = delete;

   virtual void Shutdown() override
   {
      quitting = true;
   }

   virtual ButtonResult OnAButtonReleased() override
   {
      log << log.critical << "RemoMode: AButtonReleased" << std::endl;
      lights.ToggleRed();
      return NOTHING;
   }

   virtual ButtonResult OnBButtonReleased() override
   {
      log << log.critical << "RemoMode: BButtonReleased" << std::endl;
      lights.ToggleYellow();
      return NOTHING;
   }

   virtual ButtonResult OnDButtonReleased() override
   {
      log << log.critical << "RemoMode: DButtonReleased" << std::endl;
      std::lock_guard<std::mutex> lk(mtx);
      cond.notify_one();
      quitting = true;
      return QUIT_MODE;
   }

   virtual ButtonResult OnCButtonReleased() override
   {
      log << log.critical << "RemoMode: CButtonReleased" << std::endl;
      lights.ToggleGreen();
      return NOTHING;
   }

   virtual void operator()() override;

private:
   void UpdateLights();
};

