# ML4F MicroPython module

An experiment to integrate ML4F into MicroPython.


## Build instructions

Clone this repository and initialise the git submodules (recursive submodule
init will unnecessarily initialise all submodules from all MicroPython ports):
```
git submodule update --init
git -C micropython-microbit-v2 submodule update --init
```

Changes to the micro:bit MicroPython port are required for this experiment:
```
git -C micropython-microbit-v2 apply ../micropython-microbit-v2.patch
```

Build MicroPython's mpy-cross:
```
make -C micropython-microbit-v2/lib/micropython/mpy-cross
```

And build MicroPython with the ML module (and example model) included:

```
make -C micropython-microbit-v2/src USER_C_MODULES=../../..
```

Copy the hex file to the micro:bit:
```
cp micropython-microbit-v2/src/build/MICROBIT.hex /Volumes/MICROBIT
```

Transfer the example Python code into the board with
[microFS](https://github.com/ntoll/microfs):
```
ufs put ml-module/model-example/main.py
```
