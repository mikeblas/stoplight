#pragma once

#include <condition_variable>
#include <mutex>

#include "modeinterface.h"

class ClockMode : public ModeInterface
{
   std::condition_variable cond;
   std::mutex mtx;

   int lastBeepMinute;

   void WorkLight(Lights::LightLine light, int* millisRemaining);

   int redTimeMilliseconds;
   int yellowTimeMilliseconds;
   int greenTimeMilliseconds;

public:
   ClockMode(Lights& lights, daemonize::logger& log)
      : ModeInterface(lights, log), lastBeepMinute(-1)
   {
   }

   ClockMode(const ClockMode&) = delete;

   virtual void Shutdown() override
   {
      quitting = true;
   }

   virtual ButtonResult OnAButtonReleased() override
   {
      log << log.critical << "ClockMode: AButtonReleased" << std::endl;
      lights.ToggleRed();
      return NOTHING;
   }

   virtual ButtonResult OnBButtonReleased() override
   {
      log << log.critical << "ClockMode: BButtonReleased" << std::endl;
      lights.ToggleYellow();
      return NOTHING;
   }

   virtual ButtonResult OnDButtonReleased() override
   {
      log << log.critical << "ClockMode: DButtonReleased" << std::endl;
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

