#CFLAGS = -I ~/prog/esp/ESP8266_NONOS_SDK-2.2.1/include  #-mlongcalls
#--start-group -lmain -lnet80211 -lwpa -lcirom -llwip -lpp -lphy -lc -Wl,--end-group -lgcc


XTENSA=/opt/esp-open-sdk/xtensa-lx106-elf/bin/

CC=$(XTENSA)xtensa-lx106-elf-gcc
AR=$(XTENSA)xtensa-lx106-elf-ar
LD=$(XTENSA)xtensa-lx106-elf-gcc
SIZE=$(XTENSA)xtensa-lx106-elf-size

IDIR=./inc
SDK=~/prog/esp/ESP8266_NONOS_SDK-2.2.1
#/opt/ESP8266_NONOS_SDK
LDFLAGS =$(SDK)/ld/eagle.app.v6.ld
LDLIBS = -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static -Wl,--start-group -lc -lgcc -lphy -lpp -lnet80211 -llwip -lwpa -lmain -lcrypto


.PHONY: all clean

all:
	$(CC) -I $(SDK)/include -mlongcalls -I $(IDIR) -Werror -Wl, -O0 \
	-nostdlib -c src/user_main.c -o src/user_main.o
	$(AR) cru build/app_app.a src/user_main.o
	$(LD) -L $(SDK)/lib -T $(LDFLAGS) $(LDLIBS) build/app_app.a
	-Wl,--end-group -o build/app.out
	$(SIZE) build/app.out
	esptool.py elf2image build/app.out
flash:
	esptool.py write_flash --no-compress -ff 26m -fm dio -fs 4MB-c1 0x00000 build/app.out-0x00000.bin 0x10000 build/app.out-0x10000.bin
clean:
	rm ./src/*.o ./build/*
