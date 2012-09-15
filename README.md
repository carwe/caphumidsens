caphumidsens
============

eagle-files for a pcb to convert soil moisture into frequency

You need one NE555 (or CMOS-equivalent like TLC555) in SO-8, the other parts are of size 0805.

C1=10nF
C2=100nF
R1=R2=120kOhm
D1=1N4148

The chosen values let the output oscillate with around 74kHz, when dry and in air, down to around 4kHz, when the cap-part of the board is put in water.