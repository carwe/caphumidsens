caphumidsens
============
Contains EAGLE-files for a pcb to convert soil moisture into frequency.

There are various revisions of the board:

classic design with NE555
-------------------------

###caphumidsens rev1
This was more or less a test, if the idea is working. It is quite big and unoptimized. Also the design of the capacitator is unnecessary complex.<br>
You need one NE555 (or CMOS-equivalent like TLC555) in SO-8, the other parts are of size 0805. C1=10nF; C2=100nF; R1=R2=120kOhm; D1=1N4148<br>
The chosen values let the output oscillate with around 74kHz, when dry and in air, down to around 4kHz, when the cap-part of the board is put into pure water, if you have chosen the TTL-555.

###caphumidsens rev2
...died an early death and was never physically fabricated.

###caphumidsens rev3
...is space-optimized and the components are nicely placed in a row, the draw-back is, they are too close - unfortuntely I had some of them fabricated, so it needed some fiddling to solder. <b>Do not use this layout.</b> In the discussion about this board some ideas came along: you probably want to measure the humidity in the middle of the soil, at least not directly on the surface, where it varies most. And there should be some kind of safety-margin, you don't want to change the capacitance by moving it slightly in and out of the soil. Next design should adress this.

###caphumidsens rev4
Here the capacitator was moved, so the humidity on the surface is no longer significant. Also the parts, which were too close to each other, got more space between them. In the next days, I should receive some of them from the manufacturer.

<pre>Measurements (frequency in kHz):
                Rev1/TTL        Rev1/CMOS        Rev3/CMOS        Rev4/CMOS
air              74,9              66,8            91,6              ?
dry soil         73,1              65,3            91,3              ?
half-wet soil    64,0              52,5            73,4              ?
wet soil         13,4              17,8            37,6              ?
water             3,4               ?               ?                ?</pre>


redesign with ATtiny
--------------------

###caphumidsens2 rev1 rc1/rc2/rc3
Lately I discussed about the project, and it quickly became clear, that it should be possible to connect output-pins of more than one caphumidsens-pcb over a single line with a master-controller. Using a bus has clear advantages (otherwise, you have to use a dedicated line to transport each signal, of course), so I designed a first draft. With a Attiny25/45/85 I tried to create a NE555-similar structure. A capacitator is charged to a higher voltage, then discharged to a lower voltage, and a comparator toggles the (dis-)charging. Unfortunately I could not use the internal voltage reference, as it is attached to the "wrong" side of the analog comparator. (see datasheet doc2586 Figure 16-1 "Analog Comparator Block Diagram") If I use the internal reference, I cannot use the multiplexer, and vice versa (you can compare AIN0 only with the multiplexer-input or AIN1 only with the internal reference, not AIN0/AIN1 with the multiplexer-input AND the internal reference). So I had to use an additional pin as second voltage reference (pin 3 and pin 7 as one third and two thirds of VCC - NE555-like). Pin 6 is set to VCC or GND to charge or discharge the capacitator, pin 5 senses the voltage and compares it to pin 3 or 7. On pin 1 you can talk a protocol you like - a simple frequency like the NE555 or something more complex, maybe pulling the line low against a central pull-resistor (I2C-like), sending your ID (stored in EEPROM) followed by the measured capacity. After you have written out a bit of your ID, you read it back, if another sensor is sending too (pulling the line dominates leaving the line alone, so the ID with the most "pulling bits" is the highest, CAN-like). The frequency of the messages is low enough, so that collisions should hardly only occur, and if, you simply repeat your message. Pin 1 is the reset/debugwire-pin too, so you can use debugwire to program the harder part of measuring the capacity, as you don't need the output-pin during this time. In rc3 I have added small capacitators to even out the reference voltages, if that is necessary.


