// carwe 2017
// code for caphumidsens on Digistumb (ATtiny85)
// with internal bandgap as voltage reference

#define F_CPU 8000000UL
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "BitIO.h"

void AC_setup();
void CAP_charge();
void CAP_discharge();
void ACinterrupt_handling();
void TIM_setup(char);

// on AtTiny25/45/85/13A only Port B exists
#define PIN_CHARGE	2	// as digital output
#define PIN_SENSE	0	// must be on opposite side of multiplexer-input in analog comparator -> AIN0 on AtTiny13A/25/45/85, when usage of internal voltage reference wished
				// must not be AIN0, when two external external reference voltages will be used
//#define PIN_VREF_1_3	0	// pin must be usable by analog comparator, maybe via multiplexer ; can be substituted with internal voltage reference, when above conditions for PIN_SENSE given
#define PIN_VREF_2_3	1	// pin must be usable by analog comparator, maybe via multiplexer
#define PIN_OUTPUT	3	// as digital output, for $protocol
#define PIN_OUTPUT2	4	// as digital output, for $protocol


volatile char AC_interrupt   = 0;	// 1 when ISR signals voltage equality
volatile int  AC_interrupts  = 0;	// hold the interrupts so far
volatile char CAP_charging   = 1;	// 1==charging, 0==discharging
volatile int  CAP_capacity   = 0;	// hold the interrupts per gate time
         char CAP_countAmax  = 255;	// CAP_countAmax * CAP_countBmax should be CPU cycles per gate time // and define how long charge/discharge-cycles will be added up
         int  CAP_countBmax  = 15000;	// ^
volatile int  CAP_countB     = 0;	// ^
// count from 0 to CAP_countAmax * CAP_countBmax, sum up the oscillations of the charge/discharge-cycle in AC_interrupts, and copy to CAP_capacity at the end

void main() {
	BIT_BOOL_SET(&DDRB,PIN_CHARGE,1);	// set pins as outputs
	BIT_BOOL_SET(&DDRB,PIN_OUTPUT,1);	// ^
	BIT_BOOL_SET(&DDRB,PIN_OUTPUT2,1);	// ^

	// let the LEDs blink two times to signal the end of the bootloader
	BIT_BOOL_SET(&PORTB,PIN_OUTPUT,0);
	BIT_BOOL_SET(&PORTB,PIN_OUTPUT2,0);
	_delay_ms(100);

	BIT_BOOL_SET(&PORTB,PIN_OUTPUT,1);
	BIT_BOOL_SET(&PORTB,PIN_OUTPUT2,1);
	_delay_ms(100);

	BIT_BOOL_SET(&PORTB,PIN_OUTPUT,0);
	BIT_BOOL_SET(&PORTB,PIN_OUTPUT2,0);
	_delay_ms(100);

	BIT_BOOL_SET(&PORTB,PIN_OUTPUT,1);
	BIT_BOOL_SET(&PORTB,PIN_OUTPUT2,1);
	_delay_ms(100);

	BIT_BOOL_SET(&PORTB,PIN_OUTPUT,0);
	BIT_BOOL_SET(&PORTB,PIN_OUTPUT2,0);
	_delay_ms(100);


	AC_setup();
	TIM_setup(CAP_countAmax);
	CAP_charge();


	int time = 0;
	while (1) {
		char temp1 = 0;
		BIT_BOOL_SET(&PORTB,PIN_OUTPUT,CAP_charging);
		BIT_BOOL_SET(&PORTB,PIN_OUTPUT2,(CAP_countB>(CAP_countBmax/2)));
//		BIT_BOOL_SET(&PORTB,PIN_CHARGE,(CAP_countB>(CAP_countBmax/2)));
//		BIT_BOOL_SET(&PORTB,PIN_OUTPUT2,( (&ACSR) && (1<<ACI) ));

		time+=1; if (time>=3000) { time=0; }
		_delay_ms(10);
	}
}



ISR (ANA_COMP_vect) {
	AC_interrupt=1;
	ACinterrupt_handling();
}


void AC_setup() {
	BIT_BOOL_SET(&ACSR,ACD,0);	// enable Analog Comparator
	BIT_BOOL_SET(&ADCSRB,ACME,0);	// disable multiplexer
	BIT_BOOL_SET(&ACSR,ACIS0,0);	// set to interrupt on any edge
	BIT_BOOL_SET(&ACSR,ACIS1,0);	// ^
	BIT_BOOL_SET(&ACSR,ACIE,1);	// enable AC interrupt
	sei();				// enable global interrupts
}

void CAP_charge	() {
	// AIN0 as positive side, then charge
	BIT_BOOL_SET(&ACSR,ACBG,0);

	BIT_BOOL_SET(&ACSR,ACIE,0);	// disable AC interrupt
	BIT_BOOL_SET(&ACSR,ACIS0,1);	// set to trigger on raising edge
	BIT_BOOL_SET(&ACSR,ACIS1,1);	// ^
	BIT_BOOL_SET(&ACSR,ACIE,1);	// enable AC interrupt

	BIT_BOOL_SET(&PORTB,PIN_CHARGE,1);
	CAP_charging=1;
}

void CAP_discharge ()
{
	// INTERNAL_VREF as positive side, then discharge
	BIT_BOOL_SET(&ACSR,ACBG,1);

	BIT_BOOL_SET(&ACSR,ACIE,0);	// disable AC interrupt
	BIT_BOOL_SET(&ACSR,ACIS0,0);	// set to trigger on falling edge
	BIT_BOOL_SET(&ACSR,ACIS1,1);	// ^
	BIT_BOOL_SET(&ACSR,ACIE,1);	// enable AC interrupt

	BIT_BOOL_SET(&PORTB,PIN_CHARGE,0);
	CAP_charging=0;
}

void ACinterrupt_handling()
{	// handle equality of voltages

	AC_interrupt = 0;
	if (CAP_charging)	// was charging
	{
		AC_interrupts++;
		CAP_discharge();
	} else {		// was discharging
		CAP_charge();
	}
}

void TIM_setup(char CAP_countAmax) {
	BIT_BOOL_SET(&TCCR0B,CS02,0);	// select clock source, 001==CLK without prescaler
	BIT_BOOL_SET(&TCCR0B,CS01,0);	// ^
	BIT_BOOL_SET(&TCCR0B,CS00,1);	// ^
	BIT_BOOL_SET(&TIMSK,TOIE0,1);	// overflow interrupt enable
	TCNT0 = 0;			// reset timer
	OCR0A = CAP_countAmax;		// set to overflow on this value
}

ISR (TIM0_OVF_vect) {
	// handle overflow of timer (every CAP_countAmax)

	CAP_countB++;
	if (CAP_countB >= CAP_countBmax)
	{
		CAP_countB = 0;
		CAP_capacity = AC_interrupts;
		AC_interrupts = 0;
	}
}
