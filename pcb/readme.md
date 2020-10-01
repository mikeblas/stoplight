
# Stoplight Controller PC Board

This directory contains different versions of the StopLight PC Board. I split the design since I wasn't comfortable using `git` to manage the projects, and I also wasn't positive I wouldn't fork or rework the older designs anyway. I ended up linear, though, and StopLight4 is the version of the board sent to be fabricated.

## StopLight1

Uses discrete transistors to drive the relays and indicator LEDs.

## StopLight2

Substitutes a `ULN2803` driver chip for the relays to save a lot of space.

## StopLight3

Relays turned out to be pretty noisy, so I built a TRIAC circuit to drive the lights. Maybe the `ULN2803` isn't necessary anymore, but it remains. (Uhoh. Looks like some files are missing!)

## StopLight4

There were fuses on each of the three light lines, which is a bit redundant. Seems nice, but the fuse holders take up a ton of PC board space so I've eliminated them. This resulted in a more pleasing layout and a smaller board. This board version was sent to production.

## StopLight5

This version will fix a few issues, if another production run is needed:

1) Holes in remote header too smaller (0.75mm, should be 1.00mm)
2) Phoenix connectors are facing the wrong way
3) Rotate TP5 so its silk screen matches other TPs
4) Mask over the copper legend for high-voltage outputs
5) "hash" silkscreen over high voltage area of board
6) Renumber F4 to F1


# Hardware PostMortem

Most of the interesting problems are listed above, but some are not; and some are interesting to investigate a little more.

## Power Supply

Most of the issues were with the power supply, really.

**Power Budget**. I just figured I'd get a transformer, rectifier, a couple of filter caps, and drive a 7805 and be done. It's a little more complicated than that. First off, I bugeted a bit high for power because I thought I would be holding relays up. Once I eliminated the relays, I shouldn't made a closer estimate. The supply should drive about 1.5 amps without a problem (except for heat), and I could use a much smaller transformer since my budget is really closer to 500 milliamps.

**Filter**. I couldn't find good advice about sizing the filter caps, and they're too small. Maybe the over-arching problem is that I didn't prototype the power supply on my breadboard. Had I done so, I would've caught most (if not all) of these problems.

**Heat**. My transformer develops 10 volts at the secondary, which initially sounded low. The dropout of the regulator means I need at least 7 volts at the input, and I'm making lots of heat in the linear regulator. A switching regulator is probably the best solution, and I'm probably going to end up getting one of the "cheater" switching converters on a TO-220-sized board to solve the heat issue.

## Layout

I'm pretty pleased with the layout. I was always tempted to "hide" components under the Raspberry Pi module, but I'm glad I didn't because it would  have made troubleshooting harder. Getting a probe onto anything under there would be obnoxious -- my only choice would be to get to the back of the board.

After that, the layout is pretty nice. I'm glad I got rid of the redundant fuses. It might have helped more the use an in-line fuse holder, but those also have some inconveniences. A smaller fuse form factor might have been a good idea, tho.

## In General

It was great to get rid of the relays. The clicking was completely obnoxious; I can't imagine these being used in anything but an industrial or large commercial application. Learning how to control the TRIACS was fun. 

I could probably get rid of the driver, but it's not much real estate and gives me a bit more confidence in the design for now.

## Software

The software is going well, though I don't have as much time to spend on it. When I do, I'm productive and can get some things done.

## Mechanical

The mechanical assembly came together quite well, and I'm happy with it. Even the carpentry isn't so bad.

## Light Bulbs

I'm worried that the light is too bright. Maybe a cool V2 feature would be to support PWM dimming so the brightness would be settable through software. Choosing bulbs was hard; there aren't many incadescent choices anymore. LED bulbs don't go so dim, either. The 25-watt equivalent LED bulb seems best, but it's quite expensive (about $8) and who knows how long they'll make it. A couple LED bulbs I tried didn't turn on or off correctly, so experimenting is kind of hit-and-miss.
