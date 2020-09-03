
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpiod.h>
#include <time.h>

static const char* STOPLIGHT_CLIENT = "stoplight_remo";

class Buttons
{

private:
	struct gpiod_line* line5;
	struct gpiod_line* line6;
	struct gpiod_line* line13;
	struct gpiod_line* line19;
	struct gpiod_line* line26;

	int val5;
	int val6;
	int val13;
	int val19;
	int val26;

public:
	Buttons(struct gpiod_chip* chip) {
		line5  = gpiod_chip_get_line(chip, 5);
		line6  = gpiod_chip_get_line(chip, 6);
		line13 = gpiod_chip_get_line(chip, 13);
		line19 = gpiod_chip_get_line(chip, 19);
		line26 = gpiod_chip_get_line(chip, 26);

		gpiod_line_request_input(line5, STOPLIGHT_CLIENT);
		gpiod_line_request_input(line6, STOPLIGHT_CLIENT);
		gpiod_line_request_input(line13, STOPLIGHT_CLIENT);
		gpiod_line_request_input(line19, STOPLIGHT_CLIENT);
		gpiod_line_request_input(line26, STOPLIGHT_CLIENT);
	}

	void ReadLines() {

		val5 = gpiod_line_get_value(line5);
		val6 = gpiod_line_get_value(line6);
		val13 = gpiod_line_get_value(line13);
		val19 = gpiod_line_get_value(line19);
		val26 = gpiod_line_get_value(line26);
	}

	int GetAButton() {
		return val26;
	}

	int GetBButton() {
		return val19;
	}

	int GetCButton() {
		return val13;
	}

	int GetDButton() {
		return val6;
	}

	int GetVT() {
		return val5;
	}
};

int main(void)
{

	printf("gpiod version: %s\n", gpiod_version_string());

	static const char* chipname = "gpiochip0";

	struct gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
	if (chip == nullptr) {
		perror("Open chip failed\n");
		goto end;
	}
	else
	{
		Buttons b1(chip);

		while(1) {

			b1.ReadLines();

			printf("%d, %d, %d, %d, %d\n", b1.GetAButton(), b1.GetBButton(), b1.GetCButton(), b1.GetDButton(), b1.GetVT());
			usleep(100 * 1000);

		}
	}

end:
	return 0;
}

