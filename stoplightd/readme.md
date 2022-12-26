
# stoplightd

The daemon runs the stoplight. It listens to the remote and blinks lights -- it's almost that simple!

It logs, too, to syslog. And it can run the stoplight in a bunch if different modes. Since the remote is klunky and unreliable, and because the light only has a beeper and the three traffic lights, we have to be creative about UI.

When the light is in **idle** mode, it is dark. It's not beeping, and all the lights are off. Idle mode can be exited with any button press; any button pres moves to **selection** mode.

So now let's examine the different modes:

## Selection
Selection mode lets the user select a mode.

* Binary display to select mode. A is up, B is down. When zero, red flashes 100 ms every 2 seconds (or so?)
* C selects mode.
* D exits to idle mode.
	
## Mode 0: Plain traffic light	
* A increases speed,
* B decreases speed. Speed is ratio of R48/Y8/G24 (6x, 1x, 4x), 1 second steps, 1-10
* D quits back to mode selection.

## Mode 1: Single Blinky	
* A increases speed, B decreases speed. 250ms to 5s in 250ms increments
* C sets duty cycle, looping through 100, 75, 50, 25%
* D quits back to mode selection.
	
## Mode 2: Multi Blinky
* A increases speed, B decreases speed. 250ms to 5s in 250ms increments
* C sets duty cycle, looping through 100, 75, 50, 25%
* D quits back to mode selection.

## Mode 3: Selecto
* A, B, C toggles each light
* D quits back to mode selection

## Mode 4: Clock
 * All off, but every 30 sconds shows time
 * flashes Hour, then minute in binary.
 * D quits

This is how numbers are displayed:
 * 0-7: binary; 1 = ryG, 3 = rYG, 4 = Ryg
 * 8: rapidly flashing red, y = off, g = on
 * 9: rapidly flashing red, y = on, g = off
 * each digit shown 1 second, display of 4 digits takes 4 seconds

	
## Mode 5: Weather
 * Temperature change for 2 seconds
 * Precip chance for 3 seconds  (green = 0-20%, yel= 20-60%, red > 60%)
 * 5 seconds dark
 * D quits
	
	
## Mode 6: Stock Ticker

(Not yet implemented.)


## Mode 7: Weather

(Not yet implemented.)


# Building

This project requires [libdaemonize](https://github.com/Aethelflaed/libdaemonize) and gpiod.

## libdaemonize

1. Clone the `libdaemonize` project from git.
2. It requires `libtool` to get working, so install that with `sudo apt install libtool -y`
3. In the `libdaemonize` root directory, run these commands:  
```
libtoolize
aclocal
autoconf
automake --add-missing
```
4. Now, the library can be configured and built:
```
./configure --prefix=/usr
make
```
5. And finally installed, so that the library and its headers are readable.
```
sudo make install
```
