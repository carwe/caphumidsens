// carwe 2017
// code for caphumidsens on Digistumb (ATtiny85)
// with internal bandgap as voltage reference

#define F_CPU 8000000UL
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "BitIO.h"

void setup();
void loop();
void AC_setup();
void CAP_charge();
void CAP_discharge();
void ACinterrupt_handling();

// on AtTiny25/45/85/13A only Port B exists
#define PIN_CHARGE	2	// as digital output
#define PIN_SENSE	0	// must be on opposite side of multiplexer-input in analog comparator -> AIN0 on AtTiny13A/25/45/85, when usage of internal voltage reference wished
				// must not be AIN0, when two external external reference voltages will be used
//#define PIN_VREF_1_3	0	// pin must be usable by analog comparator, maybe via multiplexer ; can be substituted with internal voltage reference, when above conditions for PIN_SENSE given
#define PIN_VREF_2_3	1	// pin must be usable by analog comparator, maybe via multiplexer
#define PIN_OUTPUT	3	// as digital output, for $protocol
#define PIN_OUTPUT2	4	// as digital output, for $protocol



volatile char CAP_charging = 1; // 1==charging, 0==discharging
volatile char AC_interrupt = 0; // 1 when ISR signals hit


void main() {
	setup();
	while (1) {
		loop();
	}
}

void setup() {
	// set pins as outputs
	BIT_SET(&DDRB,PIN_CHARGE);
	BIT_SET(&DDRB,PIN_OUTPUT);
	BIT_SET(&DDRB,PIN_OUTPUT2);

	// let the LEDs blink two times to signal the end of the bootloader
	BIT_CLEAR(&PORTB,PIN_OUTPUT);
	BIT_CLEAR(&PORTB,PIN_OUTPUT2);
	_delay_ms(100);

	BIT_SET(&PORTB,PIN_OUTPUT);
	BIT_SET(&PORTB,PIN_OUTPUT2);
	_delay_ms(100);

	BIT_CLEAR(&PORTB,PIN_OUTPUT);
	BIT_CLEAR(&PORTB,PIN_OUTPUT2);
	_delay_ms(100);

	BIT_SET(&PORTB,PIN_OUTPUT);
	BIT_SET(&PORTB,PIN_OUTPUT2);
	_delay_ms(100);

	BIT_CLEAR(&PORTB,PIN_OUTPUT);
	BIT_CLEAR(&PORTB,PIN_OUTPUT2);
	_delay_ms(100);


	AC_setup();
	CAP_charge();
}

int time;

void loop() {
//	if (AC_interrupt) {
//		ACinterrupt_handling();
//	}

	BIT_BOOL_SET(&PORTB,PIN_OUTPUT,CAP_charging);
	BIT_BOOL_SET(&PORTB,PIN_OUTPUT2,!CAP_charging);
	//BIT_BOOL_SET(&PORTB,PIN_OUTPUT2,1);


//	if (time%100 == 0) { // every 100m
//		digitalWrite(PIN_OUTPUT2, AC_interrupt);
//	}

//	if (time==0) {
//		BIT_SET(&PORTB,PIN_CHARGE);
//		BIT_SET(&PORTB,PIN_OUTPUT);
//	}


//	if(time==1500) {
//		BIT_CLEAR(&PORTB,PIN_CHARGE);
//		BIT_CLEAR(&PORTB,PIN_OUTPUT);
//	}


	time+=1; if (time>=3000) { time=0; }
	_delay_ms(1);
}


ISR (ANA_COMP_vect) {
	AC_interrupt=1;
	ACinterrupt_handling();
}


void AC_setup() {
	BIT_CLEAR(&ACSR,ACD);		// enable Analog Comparator
	BIT_CLEAR(&ADCSRB,ACME);	// disable multiplexer
	BIT_CLEAR(&ACSR,ACIS1);		// set to interrupt on any edge
	BIT_CLEAR(&ACSR,ACIS0);		// "
	BIT_SET(&ACSR,ACIE);		// enable AC interrupt
}

void CAP_charge	() {
	// AIN0 as positive side, then charge
	BIT_CLEAR(&ACSR,ACBG);
	BIT_SET(&PORTB,PIN_CHARGE);
	CAP_charging=1;
}

void CAP_discharge () {
	// INTERNAL_VREF as positive side, then discharge
	BIT_SET(&ACSR,ACBG);
	BIT_CLEAR(&PORTB,PIN_CHARGE);
	CAP_charging=0;
}

void ACinterrupt_handling() {
	// handle equality of voltages

	AC_interrupt = 0;
	if (CAP_charging) {	// was charging
		CAP_discharge();
	} else {		// was discharging
		CAP_charge();
	}
}
