
#include <gpiod.h>

#include <unistd.h>

class Lights {

    struct gpiod_line* lineRed;
    struct gpiod_line* lineYellow;
    struct gpiod_line* lineGreen;

public:
    Lights(const char* clientname, struct gpiod_chip* chip) {
     lineRed  = gpiod_chip_get_line(chip, 21);
     lineYellow  = gpiod_chip_get_line(chip, 20);
     lineGreen  = gpiod_chip_get_line(chip, 16);

     gpiod_line_request_output(lineRed, clientname, 0);
     gpiod_line_request_output(lineYellow, clientname, 0);
     gpiod_line_request_output(lineGreen, clientname, 0);
    }

    void AllOff() {
     SetRed(0);
     SetYellow(0);
     SetGreen(0);
    }

    void AllOn() {
     SetRed(1);
     SetYellow(1);
     SetGreen(1);
    }

    void SetRed(int state) {
     gpiod_line_set_value(lineRed, state);
    }

    void SetYellow(int state) {
     gpiod_line_set_value(lineYellow, state);
    }

    void SetGreen(int state) {
     gpiod_line_set_value(lineGreen, state);
    }

    void ToggleRed() {
     int state = gpiod_line_get_value(lineRed);
     SetRed(!state);
    }

    void ToggleYellow() {
     int state = gpiod_line_get_value(lineYellow);
     SetYellow(!state);
    }

    void ToggleGreen() {
     int state = gpiod_line_get_value(lineGreen);
     SetGreen(!state);
    }

    void FlickAll(int count, int millis) {

     AllOff();
     for (int n = 0; n < count; ++n) {
        usleep(1000 * millis);
        AllOn();
        usleep(1000 * millis);
        AllOff();
     }
    }
}; 


