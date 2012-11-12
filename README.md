caphumidsens
============
Contains EAGLE-files for a pcb to convert soil moisture into frequency.

There are various revisions of the board:

<b>Rev1:</b> was more or less a test, if the idea is working. It is quite big and unoptimized. Also the design of the capacitator is unnecessary complex.<br>
You need one NE555 (or CMOS-equivalent like TLC555) in SO-8, the other parts are of size 0805. C1=10nF; C2=100nF; R1=R2=120kOhm; D1=1N4148<br>
The chosen values let the output oscillate with around 74kHz, when dry and in air, down to around 4kHz, when the cap-part of the board is put into pure water, if you have chosen the TTL-555.

<b>Rev2:</b> died an early death and was never physically fabricated.

<b>Rev3:</b> is space-optimized and the components are nicely placed in a row, the draw-back is, they are too close - unfortuntely I had some of them fabricated, so it needed some fiddling to solder. <b>Do not use this layout.</b> In the discussion about this board some ideas came along: you probably want to measure the humidity in the middle of the soil, at least not directly on the surface, where it varies most. And there should be some kind of safety-margin, you don't want to change the capacitance by moving it slightly in and out of the soil. Next design should adress this.

<b>Rev4:</b> coming soon (Nov 2012)




Measurements (frequency in kHz):
		Rev1/TTL	Rev1/CMOS	Rev3/CMOS
air		74,9		66,8		91,6
dry soil	73,1		65,3		91,3
half-wet soil	64,0		52,5		73,4
wet soil	13,4		17,8		37,6
water		3,4		?		?