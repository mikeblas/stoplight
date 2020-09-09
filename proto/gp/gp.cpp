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

int main(void)
{
    printf("gpiod version: %s\n", gpiod_version_string());

    static const char* chipname = "gpiochip0";

    struct gpiod_chip* chip;
    chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        perror("Open chip failed\n");
        goto end;
    }

    gpiod_line_bulk bulk;
    int n;

    n = gpiod_chip_get_all_lines(chip, &bulk);
    if (n != 0) {
        perror("get lines failed\n");
    }
    else {

        gpiod_line *line20 = NULL;
        gpiod_line *line21 = NULL;

        for (int n = 0; n < bulk.num_lines; ++n) {
            int off = gpiod_line_offset(bulk.lines[n]);

            printf("%d: (%d) %s %d\n", n, off, gpiod_line_name(bulk.lines[n]), gpiod_line_direction(bulk.lines[n]));

            if (off == 20)
               line20 = bulk.lines[n];
            else if (off == 21)
               line21 = bulk.lines[n];
        }

        gpiod_line_request_output(line20, STOPLIGHT_CLIENT, 0);
        n = gpiod_line_request_output(line21, STOPLIGHT_CLIENT, 0);
        if (n < 0) {
            perror("Request line as output failed\n");
            goto release_line;
        }

        for (int n = 0; n < bulk.num_lines; ++n) {
            int off = gpiod_line_offset(bulk.lines[n]);

            printf("%d: (%d) %s %d\n", n, off, gpiod_line_name(bulk.lines[n]), gpiod_line_direction(bulk.lines[n]));
        }

	int ticks20 = 0;
	int ticks21 = 0;
	int state20 = 0;
	int state21 = 0;
	for (int i = 300; i > 0; i--) {

		if (ticks20 == 0) {
			printf("Output %u on line #%u\n", state20, 20);
			gpiod_line_set_value(line20, state20);
			state20 = !state20;
			ticks20 = 2 + (rand() % 11);
		}

		if (ticks21 == 0) {
			printf("Output %u on line #%u\n", state21, 21);
			gpiod_line_set_value(line21, state21);
			state21 = !state21;
			ticks21 = 2 + (rand() % 11);
		}

		ticks20--;
		ticks21--;

		usleep(100 * 1000);
	}

    }



release_line:

freeChip:
    gpiod_chip_close(chip);

end:
    return 0;
}


