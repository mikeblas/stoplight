#pragma once

#include <daemonize/syslog.hpp>

#include "lights.h"

class ModeInterface
{

protected:
   Lights& lights;
   daemonize::logger& log;
   bool quitting = false;

public:
   ModeInterface(Lights& lights, daemonize::logger& log)
      : lights(lights), log(log)
   {
   }

   virtual ~ModeInterface() = default;

   virtual void operator()() = 0;
   virtual void Shutdown() = 0;

   // -1 if this mode wants to keep running
   // otherwise, a mode number to switch to that mode
   virtual int NewMode()
   {
      return -1;
   }

   // this mode has quit running (exited)
   bool IsQuitting() const
   {
      return quitting;
   }

   enum ButtonResult {
      NOTHING,
      CHANGE_MODE,
      QUIT_MODE,
   };

   virtual ButtonResult OnAButtonPressed()  { return ButtonResult::NOTHING; }
   virtual ButtonResult OnAButtonReleased() { return ButtonResult::NOTHING; }
   virtual ButtonResult OnBButtonPressed()  { return ButtonResult::NOTHING; }
   virtual ButtonResult OnBButtonReleased() { return ButtonResult::NOTHING; }
   virtual ButtonResult OnCButtonPressed()  { return ButtonResult::NOTHING; }
   virtual ButtonResult OnCButtonReleased() { return ButtonResult::NOTHING; }
   virtual ButtonResult OnDButtonPressed()  { return ButtonResult::NOTHING; }
   virtual ButtonResult OnDButtonReleased() { return ButtonResult::NOTHING; }
};

