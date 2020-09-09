
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpiod.h>
#include <time.h>

// the gpiod library uses the BCM line numbers!

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

class Lights {

	struct gpiod_line* lineRed;
	struct gpiod_line* lineYellow;
	struct gpiod_line* lineGreen;

public:
	Lights(struct gpiod_chip* chip) {
		lineRed  = gpiod_chip_get_line(chip, 21);
		lineYellow  = gpiod_chip_get_line(chip, 20);
		lineGreen  = gpiod_chip_get_line(chip, 16);

		gpiod_line_request_output(lineRed, STOPLIGHT_CLIENT, 0);
		gpiod_line_request_output(lineYellow, STOPLIGHT_CLIENT, 0);
		gpiod_line_request_output(lineGreen, STOPLIGHT_CLIENT, 0);
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
		Lights lights(chip);
		lights.FlickAll(3, 50);

		Buttons b1(chip);
		b1.ReadLines();
		int lastVT = b1.GetVT();
		int ignores = 0;

		while(1) {

			if (ignores > 0) {
				ignores--;
			} else {

				Buttons b2(chip);
				b2.ReadLines();

				// anything change since last time?
				if (b2 != b1 || lastVT != b2.GetVT()) {

					// yes! print out the flags
					printf("b1 = %d, %d, %d, %d, %d\n", b1.GetAButton(), b1.GetBButton(), b1.GetCButton(), b1.GetDButton(), b1.GetVT());
					printf("b2 = %d, %d, %d, %d, %d\n", b2.GetAButton(), b2.GetBButton(), b2.GetCButton(), b2.GetDButton(), b2.GetVT());

					// ignore three loops to debounce
					ignores = 3;

					// if this is a release decide what to do
					if (b2.GetVT() == 1) {
						printf("***\n");
						if (b2.GetAButton()) {
							lights.ToggleRed();
						}
						if (b2.GetBButton()) {
							lights.ToggleYellow();
						}
						if (b2.GetCButton()) {
							lights.ToggleGreen();
						}
					}

					// change states
					b1 = b2;
					lastVT = b2.GetVT();
				}
			}

			usleep(100 * 1000);
		}
	}

end:
	return 0;
}

