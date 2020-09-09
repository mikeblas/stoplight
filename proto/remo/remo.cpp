
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

	int GetAButton() const {
		return val26;
	}

	int GetBButton() const {
		return val19;
	}

	int GetCButton() const {
		return val13;
	}

	int GetDButton() const {
		return val6;
	}

	int GetVT() const {
		return val5;
	}

	bool operator==(const Buttons& other) const {
		if (GetAButton() != other.GetAButton())
			return false;
		if (GetBButton() != other.GetBButton())
			return false;
		if (GetCButton() != other.GetCButton())
			return false;
		if (GetDButton() != other.GetDButton())
			return false;
		return true;
	}

	bool operator!=(const Buttons& other) const {
		return !operator==(other);
	}

	void operator=(const Buttons& other) {
		val5 = other.val5;
		val6 = other.val6;
		val19 = other.val19;
		val26 = other.val26;
		val13 = other.val13;
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
		b1.ReadLines();
		int lastVT = b1.GetVT();

		while(1) {

			Buttons b2(chip);
			b2.ReadLines();

			if (b2 != b1 || lastVT != b2.GetVT()) {

				b1 = b2;
				lastVT = b2.GetVT();

				printf("%d, %d, %d, %d, %d\n", b1.GetAButton(), b1.GetBButton(), b1.GetCButton(), b1.GetDButton(), b1.GetVT());
			}

			usleep(100 * 1000);
		}
	}

end:
	return 0;
}

