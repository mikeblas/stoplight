
#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

int main(int /* argc */, char /* **argv */)
{
	const char *chipname = "gpiochip0";
	unsigned int line_num = 5;
	struct timespec ts = { 5, 0 };
	struct gpiod_line_event event;
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int i, ret;

	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		ret = -1;
		goto end;
	}

	line = gpiod_chip_get_line(chip, line_num);
	if (!line) {
		perror("Get line failed\n");
		ret = -1;
		goto close_chip;
	}

	ret = gpiod_line_request_rising_edge_events(line, CONSUMER);
	if (ret < 0) {
		perror("Request event notification failed\n");
		ret = -1;
		goto release_line;
	}

	/* Notify event up to 20 times */
	i = 0;
	while (i <= 20) {
		ret = gpiod_line_event_wait(line, &ts);
		printf("wait returned %d\n", ret);
		if (ret < 0) {
			perror("Wait event notification failed\n");
			ret = -1;
			goto release_line;
		} else if (ret == 0) {
			printf("Wait event notification on line #%u timeout\n", line_num);
			i = 0;
			continue;
		}

		ret = gpiod_line_event_read(line, &event);
		printf("Get event notification on line #%u %d times\n", line_num, i);
		if (ret < 0) {
			perror("Read last event notification failed\n");
			ret = -1;
			goto release_line;
		}

		i++;
	}

	ret = 0;

release_line:
	gpiod_line_release(line);
close_chip:
	gpiod_chip_close(chip);
end:
	return ret;
}

