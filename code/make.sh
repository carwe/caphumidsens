#!/bin/bash

MCU=attiny85
CFLAGS=-Os
LDFLAGS=

avr-gcc ${CFLAGS}  -mmcu=${MCU} -c main.c
avr-gcc ${LDFLAGS} -mmcu=${MCU} main.o -o main.elf
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
rm main.o main.elf
micronucleus main.hex

##avrdude -c dragon_isp -p t85
