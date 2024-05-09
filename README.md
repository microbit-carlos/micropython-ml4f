# micropython-module-example
A template to create a MicroPython for micro:bit V2 C/C++ module


## Build instructions

Clone this repository, initialise the git submodules and build MicroPython's
mpy-cross:
```
git submodule update --init
git -C micropython-microbit-v2 submodule update --init
make -C micropython-microbit-v2/lib/micropython/mpy-cross
```

Build MicroPython with the C++ module included:

```
make -C micropython-microbit-v2/src USER_C_MODULES=../../..
```

Hex file: `micropython-microbit-v2/src/build/MICROBIT.hex`
