#pragma once

#include <daemonize/syslog.hpp>

#include "lights.h"

class ModeInterface
{

protected:
   Lights* pLights;
   daemonize::logger& log;
   bool quitting = false;

public:
   ModeInterface(Lights* pLights, daemonize::logger& log)
      : pLights(pLights), log(log)
      {
      }

   virtual void operator()() = 0;
   virtual void Shutdown() = 0;

   bool IsQuitting() const
   {
      return quitting;
   }

   virtual void OnAButtonPressed() { }
   virtual void OnAButtonReleased() { }
   virtual void OnBButtonPressed() { }
   virtual void OnBButtonReleased() { }
   virtual void OnCButtonPressed() { }
   virtual void OnCButtonReleased() { }
   virtual void OnDButtonPressed() { }
   virtual void OnDButtonReleased() { }
};

