from microbit import *
import time

import ml

SAMPLES_LEN = 250
TOTAL_SAMPLES = SAMPLES_LEN * 3
acc_x_y_z = [0] * TOTAL_SAMPLES

i = 0
while True:
    acc_x_y_z[i + 0] = accelerometer.get_x()
    acc_x_y_z[i + 1] = accelerometer.get_y()
    acc_x_y_z[i + 2] = accelerometer.get_z()
    i += 3
    if i >= TOTAL_SAMPLES:
        t = time.ticks_ms()
        result = ml.predict(acc_x_y_z)
        if result:
            print("t[{}] {:8s}".format(time.ticks_ms() - t, result[1][result[0]][0]), end="")
            for label_pred in result[1]:
                print(" {}[{:.2f}]".format(label_pred[0][:1], label_pred[1]), end="")
            print()
        else:
            print("t[{}] {}".format(time.ticks_ms() - t, result))
        i = 0
    sleep(20)
