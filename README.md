# stoplight
This project provides hardware and software for a stoplight controlled by a Raspberry Pi.

## Context
In the late 1990s, I purchased a traffic signal from a curiosities shop. It's a full-scale signal, appropriate for
"real" outdoor use; each lens is about 20cm in diameter, and it's a giant green beast. Until now, I never had the
time to get it working in any meaningful way.

This project includes a printed circuit board design that hosts a Raspberry Pi Zero W and implements a
mains-based power supply and switching circuits for each of the lamps in the signal. I chose the Pi Zero because
it gets me onto the wireless network, and has enough computing power to do interesting things with the signal.

The board also hosts a generic wireless remote control board built around the PT2272 receiver chip. The chip
gives four latched outputs from a four-button key fob style RF remote. This remote can be used to control
the stoplight.

And of course, the project includes software to make it do something.

## Status
This is a work-in-progress. I'l be checking in documentation and prototype projects as I go, while working out the design.

As of this writing, I'm getting used to the GPIO library on the Pi and working out how to read the RF remote output.

## Goals

I'm working on getting the hardware design finalized. With that done, I can focus on software.
The software can implement several modes, selectable by the remote. First, we can turn the thing off:

* Turn off the light; minimal processing, nothing illuminated.
* Turn the light back on; whatever mode was active last resumes.

Then, we have one of a few modes active:

* Traffic light. Red, green, yellow, in a realistically timed cycle.
* Clock. Flash lights to make a binary time-of-day clock.
* Random single flash. Flash a single (selectable) lamp at a regular interval.
* Random flash. Flash a randomly selected lamp one at a time.
* Random flash. Flash each lamp at random intervlals. 
* Weather indicator. Show tomorrow's predicted weather (temperature and precipitation)
* Stock market indicator. If the market is open, show the change in a particualr index.

## Layout

`stoplightd` is the stoplight daemon program. This is the controller that runs on the Pi (as I'm working on it)

`proto` are various fragments and scratch projects as I sort out GPIO, how the hardware works, and try out ideas.

`pcb` is the hardware project for KiCad.


# Dependencies

This project uses [libdaemonize](https://github.com/Aethelflaed/libdaemonize).



