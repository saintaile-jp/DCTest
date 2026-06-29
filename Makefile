PROJECT := dctest
TARGET := build/$(PROJECT).elf
BINARY := build/$(PROJECT).bin
BOOTFILE := disc/1ST_READ.BIN
IPBIN := build/IP.BIN
CDI := build/$(PROJECT).cdi

SOURCES := src/main.c
OBJS := $(SOURCES:src/%.c=build/%.o) romdisk.o
KOS_ROMDISK_DIR := romdisk

CFLAGS += -std=gnu11 -Wall -Wextra -Wpedantic

.PHONY: all clean run dist disc directories

all: $(TARGET)

include $(KOS_BASE)/Makefile.rules

directories:
	@mkdir -p build disc

build/%.o: src/%.c | directories
	kos-cc $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(TARGET): $(OBJS) | directories
	kos-cc -o $@ $(OBJS)

run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

dist: $(TARGET)
	$(KOS_STRIP) $(TARGET)

$(BINARY): $(TARGET)
	$(KOS_BASE)/utils/elf2bin/elf2bin $< $@ -f

$(BOOTFILE): $(BINARY) | directories
	$(KOS_BASE)/utils/scramble/scramble $< $@

$(IPBIN): ip.txt | directories
	$(KOS_BASE)/utils/makeip/makeip -f $< $@

disc: $(BOOTFILE) $(IPBIN)
	makedisc $(CDI) disc $(IPBIN) DCTEST --audio --joliet-rock

clean:
	-rm -f build/*.o build/*.elf build/*.bin build/*.cdi build/IP.BIN
	-rm -f disc/1ST_READ.BIN romdisk.img romdisk.o romdisk_tmp.c romdisk_tmp.o
