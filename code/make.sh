#!/bin/bash

TARGET=caphumidsens
MCU=attiny85
CFLAGS=-Os
LDFLAGS=


## BUILD
avr-gcc ${CFLAGS}  -mmcu=${MCU} -c main.c
avr-gcc ${LDFLAGS} -mmcu=${MCU} main.o -o ${TARGET}.elf
avr-objcopy -j .text -j .data -O ihex ${TARGET}.elf ${TARGET}.hex
avr-objdump -d ${TARGET}.elf > ${TARGET}.disassembly

## CLEANUP
rm *.o ${TARGET}.elf


## PROGRAM
micronucleus ${TARGET}.hex

##avrdude -c dragon_isp -p t85
