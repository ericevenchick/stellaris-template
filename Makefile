CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = 	-g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp \
			-Os -ffunction-sections -fdata-sections -MD -std=c99 -Wall 		\
			-pedantic -DPART_LM4F120H5QR -c -I/home/eric/code/stellarisware \
			-DTARGET_IS_BLIZZARD_RA1

LDFLAGS = -T src/lm4f120h5qr.ld --entry ResetISR -o a.out --gc-sections

OBJECTS = main.o startup_gcc.o

all: main.bin

%.o: src/%.c
	$(CC) -o $@ $^ $(CFLAGS)

a.out: $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)

main.bin: a.out
	$(OBJCOPY) -O binary $< $@

clean:
	rm *.o *.d *.bin a.out
