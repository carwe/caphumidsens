ARCHIVED on github -> migrated to codeberg: https://codeberg.org/carwe/caphumidsens


<b>code not usable due to never debugged bugs</b>

<b>code not usable due to never debugged bugs</b>

<b>code not usable due to never debugged bugs</b>

<b>code not usable due to never debugged bugs</b>




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
Here the capacitator was moved, so the humidity on the surface is no longer significant. Also the parts, which were too close to each other, got more space between them.

<pre>Measurements (frequency in kHz):
                Rev1/TTL        Rev1/CMOS        Rev3/CMOS        Rev4/CMOS
air              74,9              66,8            91,6              ?
dry soil         73,1              65,3            91,3              ?
half-wet soil    64,0              52,5            73,4              ?
wet soil         13,4              17,8            37,6              ?
water             3,4               ?               ?                ?</pre>


redesign with ATtiny
--------------------

###caphumidsens rev5
Lately I discussed about the project, and it quickly became clear, that it should be possible to connect output-pins of more than one caphumidsens-pcb over a single line with a master-controller. Using a bus has clear advantages (otherwise, you have to use a dedicated line to transport each signal, of course), so I designed a first draft.

With a ATtiny25/45/85 (maybe ATtiny13A too, if the code fits into it - comparator is existent there too, and the pinout is the same) I tried to create a NE555-similar structure. A capacitator is charged to a higher voltage, then discharged to a lower voltage, and a comparator toggles the (dis-)charging.

You have to decide if you want to use the internal voltage reference or not. If yes, you have to sense the voltage with AIN1 and compare it to the internal voltage reference and AIN0 as external voltage reference. If no, you have to sense the voltage with AIN0 and compare it witho two external voltage references on ADC0-ADC3. Some time ago I wrote here you could not compare against the internal reference and the multiplexer at the same time and save a pin - which is true, but you don't need to, it just requires another layout. To use the internal voltage reference for one side of the comparison, you have to have your sense pin as negative side on AIN1, and select the internal reference for the positive side of the AC. Then compare negative side AIN1 with AIN0 as the positive side of the AC. But, this is not what I designed.

I chose to use two external voltage references, created by division of VCC, same as in 555. This way it makes no difference what exact voltage VCC is, it can even change, the resulting frequency is independent. So you sense the voltage on AIN0 as positive side of the AC, and the multiplexer as negative side, and switch the input of the multiplexer as you charge (ADC1) and discharge (ADC2).

Pin 6 is set to VCC or GND to charge or discharge the capacitator, pin 5 senses the voltage and compares it to pin 3 or 7. On pin 1 (and maybe pin 2, if clock-ínput is not needed) you can talk a protocol you like - a simple frequency like the NE555 or something more complex, maybe pulling the line low against a central pull-resistor (I2C-like), sending your ID (stored in EEPROM) followed by the measured capacity. After you have written out a bit of your ID, you read it back, if another sensor is sending too (pulling the line dominates leaving the line alone, so the ID with the most "pulling bits" is the highest, CAN-like). The frequency of the messages is low enough, so that collisions should hardly occur, and if, you simply repeat your message.

Pin 1 is the reset/debugwire-pin too, so you can use debugwire to program the harder part of measuring the capacity, as you don't need the output-pin during this time. I have added small capacitators to even out the reference voltages, if that is necessary.

In the next days, I should receive some of them from the manufacturer.

Update five years later, 2017: yes I got them. and then I forgot them. And, I would not use Eagle anymore. Never again. Took me an hour to find an old version I could use with my licence, it brought me to tears what happened to this program after being bought by Autodesk. Everybody warned me to use this nonfree software, and I thought "whatever... works for me." - then reality hit.




