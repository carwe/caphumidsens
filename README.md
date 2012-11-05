caphumidsens
============
Contains EAGLE-files for a pcb to convert soil moisture into frequency.

There are various revisions of the board:

Rev1: was more or less a test, if the idea is working. It is quite big and unoptimized. Also the design of the capacitator is unnecessary complex.
You need one NE555 (or CMOS-equivalent like TLC555) in SO-8, the other parts are of size 0805. C1=10nF; C2=100nF; R1=R2=120kOhm; D1=1N4148
The chosen values let the output oscillate with around 74kHz, when dry and in air, down to around 4kHz, when the cap-part of the board is put in water, if you have chosen the TTL-555. No exact values for the CMOS-variant yet, but they are quite similar.

Rev2: died an early death and was never physically fabricated.

Rev3 is space-optimized and the components are nicely placed in a row, the draw-back is, they are too close - unfortuntely I had some of them fabricated, so it needed some fiddling to solder. Don not use this layout. In the discussion about this board some ideas came along: you probably want to measure the humidity in the middle of the soil, at least not directly on the surface, where it probably varies most. And there should be some kind of safety-margin, you don't want to change the capacitance by moving it slightly in and out of the soil. Next design should adress this.

Rev4: coming soon (Nov 2012)