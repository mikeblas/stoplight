// https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/tree/include/gpiod.h
// https://github.com/starnight/libgpiod-example

// http://wiringpi.com/the-gpio-utility/

// sudo apt install libgpiod-dev
// sudo apt-get install wiringpi


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpiod.h>
#include <time.h>

static const char* STOPLIGHT_CLIENT = "stoplight";

int main(int argc, char* argv[])
{
    if (argc != 4) {
usage:
        printf("Usage:\n\n");
        printf("    flick <port> <count> <periodMS>\n");
        exit(1);
    }

    int port = atoi(argv[1]);
    int count = atoi(argv[2]);
    int period = atoi(argv[3]);

    if (port < 0 || port > 32) {
        printf("port must be between 0 and 32\n");
        goto usage;
    }

    if (count <= 0 || count > 100000) {
        printf("count must be greater than zero and less than 100000\n");
        goto usage;
    }

    if (period <= 0 || period > 120000) {
        printf("period must be between 1 and 120000\n");
        goto usage;
    }

    printf("gpiod version: %s\n", gpiod_version_string());

    static const char* chipname = "gpiochip0";

    struct gpiod_chip* chip;
    chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        perror("Open chip failed\n");
        goto end;
    }
    else {

        struct gpiod_line* line = gpiod_chip_get_line(chip, port);

        int n;
        n = gpiod_line_request_output(line, STOPLIGHT_CLIENT, 0);
        if (n != 0) {
            printf("Couldn't get line %d for output\n", line);
            goto end;
        }
        else
        {
            int state = 0;
	    gpiod_line_set_value(line, state);
	    for (int k = 0; k < count*2; k++) {
		usleep(1000 * period);
        	state = !state;
	        gpiod_line_set_value(line, state);
	    }
        }
    }

release_line:

freeChip:
    gpiod_chip_close(chip);

end:
    return 0;
}


