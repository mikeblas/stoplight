#pragma once

#include <gpiod.h>

#include <unistd.h>

#include <unordered_map>
#include <chrono>

using namespace std::chrono;

static milliseconds GetNowMillis()
{
    milliseconds ms = duration_cast< milliseconds > (
        system_clock::now().time_since_epoch()
    );
    return ms;
}



struct OnOffCounter {
    OnOffCounter(milliseconds startedMillis)
    :   timeOnMillis(0),
        timeOffMillis(0),
        stateStartedTime(startedMillis),
        currentState(0)
    {
    }

    OnOffCounter(const OnOffCounter& other)
    :   timeOnMillis(other.timeOnMillis),
        timeOffMillis(other.timeOffMillis),
        stateStartedTime(other.stateStartedTime),
        currentState(other.currentState)
    {
    }

    constexpr OnOffCounter& operator=(const OnOffCounter&) = default;

    OnOffCounter()
    :   timeOnMillis(0),
        timeOffMillis(0),
        stateStartedTime(0),
        currentState(0)
    {
    }

    // change the state we reprsent, accumulating the time
    void UpdateState(int state)
    {
      if (currentState != state)
      {
          milliseconds now = GetNowMillis();
          long diff = (now - stateStartedTime).count();
          if (currentState)
              timeOnMillis += diff;
          else
              timeOffMillis += diff;
          stateStartedTime = now;
          currentState = state;
      }
    }

    long timeOnMillis;
    long timeOffMillis;
    milliseconds stateStartedTime;
    int currentState;
};

class Lights {

    struct gpiod_line* lineRed;
    struct gpiod_line* lineYellow;
    struct gpiod_line* lineGreen;
    struct gpiod_line* lineInd1;
    struct gpiod_line* lineInd2;

public:
   enum LightLine {
      LIGHT_RED,
      LIGHT_YELLOW,
      LIGHT_GREEN,

      LIGHT_IND_1,
      LIGHT_IND_2,
   };

protected:
    std::unordered_map<LightLine, OnOffCounter> timingMap;


public:
   Lights(const char* clientname, struct gpiod_chip* chip)
   {
      lineRed    = gpiod_chip_get_line(chip, 21);
      lineYellow = gpiod_chip_get_line(chip, 20);
      lineGreen  = gpiod_chip_get_line(chip, 16);

      lineInd1 = gpiod_chip_get_line(chip, 12);
      lineInd2 = gpiod_chip_get_line(chip, 25);

      gpiod_line_request_output(lineRed, clientname, 0);
      gpiod_line_request_output(lineYellow, clientname, 0);
      gpiod_line_request_output(lineGreen, clientname, 0);

      gpiod_line_request_output(lineInd1, clientname, 0);
      gpiod_line_request_output(lineInd2, clientname, 0);

      milliseconds now = GetNowMillis();

      timingMap[LIGHT_RED   ] = OnOffCounter(now);
      timingMap[LIGHT_GREEN ] = OnOffCounter(now);
      timingMap[LIGHT_YELLOW] = OnOffCounter(now);
      timingMap[LIGHT_IND_1 ] = OnOffCounter(now);
      timingMap[LIGHT_IND_2 ] = OnOffCounter(now);
   }

   const OnOffCounter& GetLightCounter(Lights::LightLine line)
   {
      return timingMap[line];
   }

   void AllOff()
   {
      SetRed(0);
      SetYellow(0);
      SetGreen(0);

      SetInd1(0);
      SetInd2(0);
   }

   void AllOn()
   {
      SetRed(1);
      SetYellow(1);
      SetGreen(1);

      SetInd1(1);
      SetInd2(1);
   }

   void SetRed(int state)
   {
      gpiod_line_set_value(lineRed, state);
      timingMap[LIGHT_RED].UpdateState(state);
   }

   void SetYellow(int state)
   {
      gpiod_line_set_value(lineYellow, state);
      timingMap[LIGHT_YELLOW].UpdateState(state);
   }

   void SetGreen(int state)
   {
      gpiod_line_set_value(lineGreen, state);
      timingMap[LIGHT_GREEN].UpdateState(state);
   }

   void SetInd1(int state)
   {
      gpiod_line_set_value(lineInd1, state);
      timingMap[LIGHT_IND_1].UpdateState(state);
   }

   void SetInd2(int state)
   {
      gpiod_line_set_value(lineInd2, state);
      timingMap[LIGHT_IND_2].UpdateState(state);
   }

   void ToggleRed()
   {
      int state = gpiod_line_get_value(lineRed);
      SetRed(!state);
   }

   void ToggleYellow()
   {
      int state = gpiod_line_get_value(lineYellow);
      SetYellow(!state);
   }

   void ToggleGreen()
   {
      int state = gpiod_line_get_value(lineGreen);
      SetGreen(!state);
   }

   void ToggleInd1()
   {
      int state = gpiod_line_get_value(lineInd1);
      SetInd1(!state);
   }

   void ToggleInd2()
   {
      int state = gpiod_line_get_value(lineInd2);
      SetInd2(!state);
   }

   void Toggle(enum LightLine light)
   {
      switch (light)
      {
         case LIGHT_RED:
            ToggleRed();
            break;

         case LIGHT_GREEN:
            ToggleGreen();
            break;

         case LIGHT_YELLOW:
            ToggleYellow();
            break;

         case LIGHT_IND_1:
            ToggleInd1();
            break;

         case LIGHT_IND_2:
            ToggleInd2();
            break;
      }
   }


   void OffNum(enum LightLine light)
   {
      SetNum(light, 0);
   }

   void OnNum(enum LightLine light)
   {
      SetNum(light, 1);
   }


   void SetNum(enum LightLine light, int state)
   {

      switch (light)
      {
         case LIGHT_RED:
            SetRed(state);
            break;

         case LIGHT_GREEN:
            SetGreen(state);
            break;

         case LIGHT_YELLOW:
            SetYellow(state);
            break;

         case LIGHT_IND_1:
            SetInd1(state);
            break;

         case LIGHT_IND_2:
            SetInd2(state);
            break;
      }
   }

   void FlickAll(int count, int millis) {

      AllOff();
      for (int n = 0; n < count; ++n)
      {
         usleep(1000 * millis);
         AllOn();
         usleep(1000 * millis);
         AllOff();
      }
   }
};
