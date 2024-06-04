from microbit import *
import time

import ml

ml.start()
labels = ml.get_labels()

while True:
    gesture = ml.get_result()
    if gesture == "Jumping":
        display.show("J")
    if gesture == "Running":
        display.show("R")
    if gesture == "Walking":
        display.show("W")
    if gesture == "Standing":
        display.show("S")

    if button_a.is_pressed():
        if ml.is_running():
            ml.end()
        else:
            ml.start()

    if button_b.is_pressed():
        display.clear()

    sleep(100)
