#!/bin/bash

TARGET=caphumidsens
MCU=attiny85
#CFLAGS="-Os -g"
CFLAGS="-Os"
LDFLAGS=


## BUILD
avr-gcc ${CFLAGS}  -mmcu=${MCU} -c main.c
avr-gcc ${LDFLAGS} -mmcu=${MCU} main.o -o ${TARGET}.elf
#cp ${TARGET}.elf ${TARGET}_dbg.elf
#avr-strip  --strip-debug ${TARGET}.elf
avr-objcopy -j .text -j .data -O ihex ${TARGET}.elf ${TARGET}.hex
avr-objdump -d ${TARGET}.elf > ${TARGET}.disassembly

## CLEANUP
rm *.o ${TARGET}.elf
#chmod a-x ${TARGET}.elf ${TARGET}_dbg.elf

## PROGRAM
micronucleus ${TARGET}.hex

##avrdude -c dragon_isp -p t85
