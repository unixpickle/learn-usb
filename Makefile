CFLAGS=$(shell pkg-config --cflags --libs libusb-1.0)

all: build build/descriptors build/keyboard

build/descriptors: descriptors/main.c
	$(CC) -o $@ $^ $(CFLAGS)

build/keyboard: keyboard/main.c
	$(CC) -o $@ $^ $(CFLAGS)

build:
	mkdir build

clean:
	rm -rf build