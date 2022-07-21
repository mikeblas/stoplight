# stoplight
This project provides hardware and software for a stoplight controlled by a Raspberry Pi.

## Context
In the late 1990s, I purchased a traffic signal from a curiosities shop. It's a full-scale signal, appropriate for
"real" outdoor use; each lens is about 20cm in diameter, and it's a giant green beast. During the lockdowns in 2020,
I finally found the time to get it working in a fun and useful way.

This project includes a printed circuit board design that hosts a Raspberry Pi Zero W and implements a
mains-based power supply and switching circuits for each of the lamps in the signal. I chose the Pi Zero because
it gets me onto the wireless network, and has enough computing power to do interesting things with the signal. With
a little research, it should be possible to find a smaller board that has WiFi connectivity, but I already had an
unused Pi Zero, so I just went with it.

The board further hosts a generic wireless remote control board built around the PT2272 receiver chip. The chip
gives four latched outputs from a four-button key fob style RF remote. This remote can be used to control
the stoplight.

And of course, the project includes software to make it do something.

## Status

This project is "done enough" for now. All the features below are implemented, and the wish list features are
not yet implemented.

## Hardware Features

The hardware is done; amazingly, I didn't have to re-print the board and got it right on the first try. The
only problem was using a 7805 regulator. The voltage drop made it run red hot, so I replaced it with a three-pin
PC-based switching regulator, which works fine. The filter capacitor on the output of the supply wasn't adequate,
so I increased it. That, of course, required a part with a bigger footprint so the mounting is a bit lumpy,
but it works fine.

## Software Features

The software implements several modes, selectable by the remote. First, we can turn the thing off:

* Turn off the light; minimal processing, nothing illuminated.
* Turn the light back on; whatever mode was active last resumes.

Then, we have one of a few modes active:

* Traffic light. Red, green, yellow, in a realistically timed cycle.
* Random single flash. Flash a single (selectable) lamp at a regular interval.
* Random flash. Flash a randomly selected lamp one at a time.
* Random flash. Flash each lamp at random intervals. This is "disco mode"! 
* Manual selection. Turn a lamp off or on, and it stays that way.

We mostly leave the light in disco mode, and it's fun to see it blinking away.

## Wish List

I'd like to add a few more modes:

* Clock. Flash lights to make a binary time-of-day clock.
* Weather indicator. Show tomorrow's predicted weather (temperature and precipitation)
* Stock market indicator. If the market is open, show the directional change in a particular index.

There's also a couple of interesting things that I'd like to add:

* Mode memory. It would be great to have the system restore its mode when it starts up again after
a reboot or power failure.
* Monitoring. Anything I can monitor in my home network is monitored -- lots of SNMP collectors and
some Influx scripts pull data in to Grafana. Adding 
* Web page. It would be cool to have a little web server to present a UI for changing modes, looking
at statistics, and so on.
* Mobile app. A web API would enable a mobile application for modes and stats, too.

Since the project is built and installed, then I'm not very motivated to work on these things. Maybe
in the winter I'll get another Pi Zero and set it up so I can run the core on my bench, and get to
work on those feature. The design has on-board LEDs for all of the outputs, so pretty much no support
circuitry is necessary. I just need to build and populate one of my spare PC boards. 

## Project Layout

`stoplightd` is the stoplight daemon program. This is the controller that runs on the Pi (as I'm working on it)

`proto` are various fragments and scratch projects as I sort out GPIO, how the hardware works, and try out ideas.

`pcb` is the hardware project for KiCad.

## Images

Installing KiCad just to take a peek is a big ask, so here are some links:

* A snapshot of [the PC board](docs/pics/pc_board.png "PC Board")
* A snapshot of [the schematic](docs/pics/schematic.png "Schematic")


# Dependencies

This project uses [libdaemonize](https://github.com/Aethelflaed/libdaemonize).



