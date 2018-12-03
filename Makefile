CFLAGS=$(shell pkg-config --cflags --libs libusb-1.0) -Icommon -lm

all: build build/descriptors build/keyboard build/mass_storage

build/descriptors: descriptors/main.c
	$(CC) -o $@ $^ $(CFLAGS)

build/keyboard: keyboard/main.c common/find_interfaces.c
	$(CC) -o $@ $^ $(CFLAGS)

build/mass_storage: mass_storage/main.c mass_storage/commands.c mass_storage/scsi.c common/find_interfaces.c
	$(CC) -o $@ $^ $(CFLAGS)

build:
	mkdir build

clean:
	rm -rf build