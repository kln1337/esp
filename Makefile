#CFLAGS = -I ~/prog/esp/ESP8266_NONOS_SDK-2.2.1/include  #-mlongcalls
#--start-group -lmain -lnet80211 -lwpa -lcirom -llwip -lpp -lphy -lc -Wl,--end-group -lgcc


XTENSA=/opt/esp-open-sdk/xtensa-lx106-elf/bin/

CC=$(XTENSA)xtensa-lx106-elf-gcc
AR=$(XTENSA)xtensa-lx106-elf-ar
LD=$(XTENSA)xtensa-lx106-elf-gcc
SIZE=$(XTENSA)xtensa-lx106-elf-size

IDIR=./inc
SDK=~/prog/esp/ESP8266_NONOS_SDK-2.2.1
SDK_DRIVER=$(SDK)/driver_lib

LDFILE =$(SDK)/ld/eagle.app.v6.ld
LDFLAGS = -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static -Wl,--start-group -lc -lgcc -lphy -lpp -lnet80211 -llwip -lwpa -lmain -lcrypto
FLASHER=$(SDK)/tools/esptool.py
CFLAGS=-Werror -Wl, -O0 -nostdlib -mlongcalls -std=c99
INCLUDE=-I $(IDIR) -I $(SDK)/include -I $(SDK_DRIVER)/include

.PHONY: all clean

all: build/app.out
	$(SIZE) build/app.out
	$(FLASHER) --chip esp8266 elf2image build/app.out

src/common_funcs.o: src/common_funcs.c $(IDIR)/common_funcs.h
	@echo "src/common_funcs.o"
	$(CC) $(INCLUDE) $(CFLAGS) -c src/common_funcs.c -o src/common_funcs.o

src/user_main.o: src/user_main.c $(IDIR)/common_funcs.h $(IDIR)/user_config.h
	@echo "src/user_main.o"
	$(CC) $(INCLUDE) $(CFLAGS) -c src/user_main.c -o src/user_main.o

build/app_app.a: src/user_main.o src/common_funcs.o
	@echo "AR"
	$(AR) cur build/app_app.a src/user_main.o src/common_funcs.o

build/app.out: build/app_app.a
	@echo "LD"
	$(LD) -L $(SDK)/lib -T $(LDFILE) $(LDFLAGS) build/app_app.a $(SDK)/lib/libdriver.a -Wl,--end-group -o build/app.out

flash:
	$(FLASHER) write_flash -fs 4MB-c1 --no-compress 0x00000 build/app.out-0x00000.bin 0x10000 build/app.out-0x10000.bin
clean:
	rm ./src/*.o ./build/*

# $(CC) -I $(SDK)/include -mlongcalls -I $(IDIR) -I $(SDK_DRIVER)/include -Werror -Wl, -O0 \
# 	-nostdlib -c src/user_main.c -o src/user_main.o
# 	$(AR) cru build/app_app.a src/user_main.o
# 	$(LD) -L $(SDK)/lib -T $(LDFILE) $(LDFLAGS) $(SDK)/lib/libdriver.a build/app_app.a -Wl,--end-group -o build/app.out
