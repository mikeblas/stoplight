
# Stoplight Controller PC Board

This directory contains different versions of the StopLight PC Board. I split the design since I wasn't comfortable using `git` to manage the projects, and I also wasn't positive I wouldn't fork or rework the older designs anyway. I ended up linear, though, and StopLight4 is the version of the board sent to be fabricated.


## StopLight1

Uses discrete transistors to drive the relays and indicator LEDs.


## StopLight2

Substitutes a `ULN2803` driver chip for the relays to save a lot of space.


## StopLight3

Relays turned out to be pretty noisy, so I built a TRIAC circuit to drive the lights. Maybe the `ULN2803` isn't necessary anymore, but it remains. (Uhoh. Looks like some files are missing!)

## StopLight4

There were fuses on each of the three light lines, which is a bit redundant. Seems nice, but the fuse holders take up a ton of PC board space so I've eliminated them. This resulted in a more pleasing layout and a smaller board.


