# Overview

This is a simple keyboard driver. All it does is print out the keypresses from a USB keyboard.

The driver puts the USB keyboard into legacy mode, which is designed for BIOSes and other simple pieces of software. See the [USB HID specification](https://usb.org/sites/default/files/documents/hid1_11.pdf) for more.

# Example output

Here's an example of what you might see when running the program. On my system, for some reason my mouse exposes an interface that looks like a keyboard. I simply hit `n` for that device.

```
$ ./build/keyboard
Found device (bus=2 addr=9)
Use name=Wireless Device manufacturer=Chicony? [y/n]: n
Found device (bus=2 addr=2)
Use name=DasKeyboard manufacturer=? [y/n]: y
dumping keys (press QWERTY Ctrl+C to quit)...
modifiers=0 key=4 press=1
modifiers=0 key=4 press=0
modifiers=0 key=17 press=1
modifiers=0 key=17 press=0
modifiers=0 key=4 press=1
modifiers=0 key=4 press=0
modifiers=0 key=49 press=1
modifiers=0 key=49 press=0
modifiers=1 key=12 press=1
modifiers=1 key=12 press=0
modifiers=1 key=6 press=1
```
