#include <gpiod.h>


class Buzzer
{
   struct gpiod_line* lineBuzz;

public:
   Buzzer(const char* clientname, struct gpiod_chip* chip)
   {

      lineBuzz = gpiod_chip_get_line(chip, 25);
      gpiod_line_request_output(lineBuzz, clientname, 0);
   }

   void On()
   {
           gpiod_line_set_value(lineBuzz, 1);
   }

   void Off()
   {
           gpiod_line_set_value(lineBuzz, 0);
   }
};

