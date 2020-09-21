#pragma once

#include "buzzer.h"


class SmartBuzzer : public Buzzer
{

   enum State {
      IDLE,
      ONE_BEEP_ON,
      TWO_BEEPS_OFF,
      TWO_BEEPS_ON,
      THREE_BEEPS_OFF,
      THREE_BEEPS_ON,
   };

   State currentState = IDLE;

   int ticksLeft = 0;
   static const int halfDurationTicks = 1;

public:
   SmartBuzzer(const char* clientname, struct gpiod_chip* chip)
   :  Buzzer(clientname, chip)
   {
   }

   ~SmartBuzzer() = default;

   void Advance()
   {
      if (currentState != IDLE)
      {
         if (--ticksLeft == 0)
            ChooseNewState();
         SetBuzzer();
      }
   }

   void ThreeBeeps()
   {
      currentState = THREE_BEEPS_ON;
      ticksLeft = halfDurationTicks;
      SetBuzzer();
   }

   void TwoBeeps()
   {
      currentState = TWO_BEEPS_ON;
      ticksLeft = halfDurationTicks;
      SetBuzzer();
   }

   void OneBeep()
   {
      currentState = ONE_BEEP_ON;
      ticksLeft = halfDurationTicks;
      SetBuzzer();
   }



protected:

   void SetBuzzer()
   {
      switch (currentState)
      {
         case IDLE:
         case TWO_BEEPS_OFF:
         case THREE_BEEPS_OFF:
         default:
            Off();
            break;

         case TWO_BEEPS_ON:
         case ONE_BEEP_ON:
         case THREE_BEEPS_ON:
            On();
            break;
      }
   }

   void ChooseNewState()
   {
      switch (currentState)
      {
         case IDLE:
            break;

         case TWO_BEEPS_OFF:
            currentState = ONE_BEEP_ON;
            ticksLeft = halfDurationTicks;
            break;

         case THREE_BEEPS_OFF:
            currentState = TWO_BEEPS_ON;
            ticksLeft = halfDurationTicks;
            break;

         case TWO_BEEPS_ON:
            currentState = TWO_BEEPS_OFF;
            ticksLeft = halfDurationTicks;
            break;

         case ONE_BEEP_ON:
            currentState = IDLE;
            ticksLeft = halfDurationTicks;
            break;

         case THREE_BEEPS_ON:
            currentState = THREE_BEEPS_OFF;
            ticksLeft = halfDurationTicks;
            break;
      }
   }

};



