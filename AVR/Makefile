DEVICE=atmega16a
CFLAGS= -Wall -W -s -O3 -mmcu=${DEVICE} -DF_CPU=14745600UL
LFLAGS= -s -mmcu=${DEVICE} -Wl,-Map=bin/Release/AVR.map,--cref -s
INCLUDE= .

.PHONY: Release clean cleanRelease AVRsize

Release: bin/Release/AVR.hex bin/Release/AVR.lss bin/Release/AVR.eep bin/Release/AVR.lock bin/Release/AVR.sig AVRsize

bin/Release/AVR.hex: bin/Release/AVR.elf
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex bin/Release/AVR.elf bin/Release/AVR.hex

bin/Release/AVR.lss: bin/Release/AVR.elf
	avr-objdump -h -S bin/Release/AVR.elf > bin/Release/AVR.lss

bin/Release/AVR.eep: bin/Release/AVR.elf
	avr-objcopy --no-change-warnings -j .eeprom --change-section-lma .eeprom=0 -O ihex bin/Release/AVR.elf bin/Release/AVR.eep

bin/Release/AVR.lock: bin/Release/AVR.elf
	avr-objcopy --no-change-warnings -j .lock --change-section-lma .lock=0 -O ihex bin/Release/AVR.elf bin/Release/AVR.lock

bin/Release/AVR.sig: bin/Release/AVR.elf
	avr-objcopy --no-change-warnings -j .signature --change-section-lma .signature=0 -O ihex bin/Release/AVR.elf bin/Release/AVR.sig

bin/Release/AVR.fuse: bin/Release/AVR.elf
	avr-objcopy --no-change-warnings -j .fuse --change-section-lma .fuse=0 -O ihex bin/Release/AVR.elf bin/Release/AVR.fuse

bin/Release/AVR.elf: obj/Release/main.o
	avr-g++ ${LFLAGS} -o bin/Release/AVR.elf obj/Release/main.o

obj/Release/main.o: main.c
	avr-gcc ${CFLAGS} -I${INCLUDE} -c main.c -o obj/Release/main.o

AVRsize: bin/Release/AVR.elf
	avr-size bin/Release/AVR.elf

clean: cleanRelease

cleanRelease:
	rm -f bin/Release/* obj/Release/*
