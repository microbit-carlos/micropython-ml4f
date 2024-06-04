#include <py/runtime.h>
#include "mlinterface.h"

mp_obj_t get_input_length_func(void) {
    return mp_obj_new_int(ml_getInputLength());
}
static MP_DEFINE_CONST_FUN_OBJ_0(get_input_length_func_obj, get_input_length_func);

mp_obj_t get_labels_func(void) {
    ml_labels_t* labels = ml_getLabels();
    if (labels == NULL) {
        return mp_const_none;
    }
    mp_obj_t tup_labels[labels->num_labels];
    for (int i = 0; i < labels->num_labels; i++) {
        tup_labels[i] = mp_obj_new_str(
            labels->labels[i],
            strlen(labels->labels[i])
        );
    }
    return mp_obj_new_tuple(labels->num_labels, tup_labels);
}
static MP_DEFINE_CONST_FUN_OBJ_0(get_labels_func_obj, get_labels_func);


mp_obj_t predict_func(mp_obj_t x_y_z_obj) {
    // TODO: Expand the types of input accepted
    if (!mp_obj_is_type(x_y_z_obj, &mp_type_list)) {
        // TODO: Use a better exception type
        mp_raise_ValueError(MP_ERROR_TEXT("Input data must be a list."));
    }
    size_t input_len;
    mp_obj_t *input_items;
    mp_obj_list_get(x_y_z_obj, &input_len, &input_items);

    if (!ml_isModelPresent()) {
        mp_raise_ValueError(MP_ERROR_TEXT("Cannot find model in flash."));
    }

    const size_t model_input_num = ml_getInputLength();

    if (input_len != model_input_num) {
        mp_raise_ValueError(MP_ERROR_TEXT("Input data number of elements invalid."));
    }

    // Convert the values from int milli-g to float g
    float x_y_z[model_input_num];
    for (int i = 0; i < model_input_num; i++) {
        x_y_z[i] = (float)(mp_obj_get_int(input_items[i]) / 1000.0);
    }

    ml_prediction_t* predictions = ml_predict(x_y_z);
    if (predictions == NULL) {
        return mp_const_none;
    }

    // Create a tuple with tuples of (label, prediction_value)
    mp_obj_t tup_values[predictions->num_labels];
    for (int i = 0; i < predictions->num_labels; i++) {
        tup_values[i] = mp_obj_new_tuple(2, (mp_obj_t[]){
            mp_obj_new_str(predictions->labels[i],
                           strlen(predictions->labels[i])),
            mp_obj_new_float(predictions->predictions[i]),
        });
    }
    mp_obj_t tuple_values = mp_obj_new_tuple(predictions->num_labels, tup_values);

    // And a tuple with the index of the max value and the tuple of labels+predictions
    return mp_obj_new_tuple(2, (mp_obj_t[]){
        mp_obj_new_int(predictions->max_index),
        tuple_values,
    });
}
static MP_DEFINE_CONST_FUN_OBJ_1(predict_func_obj, predict_func);


// Init state is stored on mp_state so that it is cleared on soft reset.
// TODO: This doesn't seem to be working.
MP_REGISTER_ROOT_POINTER(int ml_initialised);

static mp_obj_t ml___init__(void) {
    if (!MP_STATE_VM(ml_initialised)) {
        // __init__ for builtins is called each time the module is imported,
        //   so ensure that initialisation only happens once
        MP_STATE_VM(ml_initialised) = true;
        DEBUG_PRINT("ml.__init_ called\n");

        // Set the model to use for inference
        bool success = ml_loadModel();
        if (!success) {
            DEBUG_PRINT("Failed to load model\n");
        } else {
            DEBUG_PRINT("Model successfully loaded\n");
        }
    } else {
        DEBUG_PRINT("ml.__init_ already initialised\n");
        bool model_loaded = ml_isModelPresent();
        DEBUG_PRINT("Model %s loaded\n", model_loaded ? "already" : "not");
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(ml___init___obj, ml___init__);


// Define all attributes of the module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
static const mp_rom_map_elem_t ml_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ml) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&ml___init___obj) },
    { MP_ROM_QSTR(MP_QSTR_get_input_length), MP_ROM_PTR(&get_input_length_func_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_labels), MP_ROM_PTR(&get_labels_func_obj) },
    { MP_ROM_QSTR(MP_QSTR_predict), MP_ROM_PTR(&predict_func_obj) },
};
static MP_DEFINE_CONST_DICT(ml_module_globals, ml_module_globals_table);

// Define module object.
const mp_obj_module_t ml_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&ml_module_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_ml, ml_cmodule);
