
import ml_runner as _ml_runner
import microbit as _mb
from ml_runner import get_labels


_running = False
_acc_x_y_z = []
_acc_i = 0
_total_samples = 0
_last_results = []


def _capture_data():
    global _acc_x_y_z, _acc_i, _running
    if not _running:
        return

    _acc_x_y_z[_acc_i + 0] = _mb.accelerometer.get_x()
    _acc_x_y_z[_acc_i + 1] = _mb.accelerometer.get_y()
    _acc_x_y_z[_acc_i + 2] = _mb.accelerometer.get_z()
    _acc_i += 3

    if _acc_i >= _total_samples:
        t = _mb.running_time()
        result = _ml_runner.predict(_acc_x_y_z)
        if result:
            prediction_label = result[1][result[0]][0]
            _last_results.append(prediction_label)
            print("t[{}] {:8s}".format(_mb.running_time() - t, prediction_label), end="")
            for label_pred in result[1]:
                print(" {}[{:.2f}]".format(label_pred[0][:1], label_pred[1]), end="")
            print()
        else:
            print("t[{}] ERROR! {}".format(_mb.running_time() - t, result))
        _acc_i = 0


def start():
    print("Input size: {}".format(_ml_runner.get_input_length()))
    print("Model labels: {}".format(_ml_runner.get_labels()))

    global _running, _acc_x_y_z, _acc_i, _total_samples
    _total_samples = _ml_runner.get_input_length()
    _acc_x_y_z = [0] * _total_samples
    _acc_i = 0
    _running = True

    _mb.run_every(_capture_data, ms=25)


def end():
    global _running
    _running = False


def is_running():
    return _running


def get_results():
    global _last_results
    results = _last_results
    _last_results = []
    return results


def get_result():
    global _last_results
    last = None
    if _last_results:
        last = _last_results[-1]
        _last_results = []
    return last


def was(label):
    global _last_results
    result = label in _last_results
    _last_results = []
    return result
