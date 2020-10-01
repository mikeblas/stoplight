#pragma once

#include <gpiod.h>

#include <unistd.h>

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

   Lights(const char* clientname, struct gpiod_chip* chip)
   {
      lineRed  = gpiod_chip_get_line(chip, 21);
      lineYellow  = gpiod_chip_get_line(chip, 20);
      lineGreen  = gpiod_chip_get_line(chip, 16);

      lineInd1 = gpiod_chip_get_line(chip, 12);
      lineInd2 = gpiod_chip_get_line(chip, 25);

      gpiod_line_request_output(lineRed, clientname, 0);
      gpiod_line_request_output(lineYellow, clientname, 0);
      gpiod_line_request_output(lineGreen, clientname, 0);

      gpiod_line_request_output(lineInd1, clientname, 0);
      gpiod_line_request_output(lineInd2, clientname, 0);
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
   }

   void SetYellow(int state)
   {
      gpiod_line_set_value(lineYellow, state);
   }

   void SetGreen(int state)
   {
      gpiod_line_set_value(lineGreen, state);
   }

   void SetInd1(int state)
   {
      gpiod_line_set_value(lineInd1, state);
   }

   void SetInd2(int state)
   {
      gpiod_line_set_value(lineInd2, state);
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


