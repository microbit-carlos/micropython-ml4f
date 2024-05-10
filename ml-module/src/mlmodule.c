#include <py/objstr.h>

#include "ml4f.h"
#include "model_example.h"

float* ml4f_invoke_example_model(const float *input, const int len) {
    static float out[ml4f_model_example_num_labels];
    int r = ml4f_full_invoke((ml4f_header_t *)ml4f_model_example, input, out);
    if (r != 0) {
        return NULL;
    }
    return (float *)out;
}

// Here we implement the function using C++ code, but since it's
// declaration has to be compatible with C everything goes in extern "C" scope.
mp_obj_t cppfunc(mp_obj_t a_obj, mp_obj_t b_obj) {
    // The following no-ops are just here to verify the static assertions used in
    // the public API all compile with C++.
    MP_STATIC_ASSERT_STR_ARRAY_COMPATIBLE;
    if (mp_obj_is_type(a_obj, &mp_type_BaseException)) {
    }

    // Prove we have (at least) C++11 features.
    const int a = mp_obj_get_int(a_obj);
    const int b = mp_obj_get_int(b_obj);
    const mp_obj_t sum = mp_obj_new_int(a + b);
    // Prove we're being scanned for QSTRs.
    mp_obj_t tup[] = {sum, MP_ROM_QSTR(MP_QSTR_hellocpp)};
    return mp_obj_new_tuple(2, tup);
}
// Define a Python reference to the function we'll make available.
// See example.cpp for the definition.
static MP_DEFINE_CONST_FUN_OBJ_2(cppfunc_obj, cppfunc);

mp_obj_t predict_func(mp_obj_t x_y_z_obj) {
    // The following no-ops are just here to verify the static assertions used in
    // the public API all compile with C++.
    MP_STATIC_ASSERT_STR_ARRAY_COMPATIBLE;
    if (!mp_obj_is_type(x_y_z_obj, &mp_type_list)) {
        return mp_const_none;
    }

    // Get the list items and length
    size_t len;
    mp_obj_t *items;
    mp_obj_list_get(x_y_z_obj, &len, &items);

    if (len != ml4f_model_example_input_num_elements) {
        return mp_const_none;
    }

    // Convert the values from int milli-g to float g
    static float x_y_z[ml4f_model_example_input_num_elements];
    for (int i = 0; i < ml4f_model_example_input_num_elements; i++) {
        x_y_z[i] = (float)(mp_obj_get_int(items[i]) / 1000.0);
    }

    float* out_values = ml4f_invoke_example_model(x_y_z, ml4f_model_example_input_num_elements);
    if (out_values == NULL) {
        return mp_const_none;
    }
    int max_index = ml4f_argmax(out_values, ml4f_model_example_num_labels);

    // Create a tuple with each label prediction value
    mp_obj_t tup_values[ml4f_model_example_num_labels];
    for (int i = 0; i < ml4f_model_example_num_labels; i++) {
        tup_values[i] = mp_obj_new_float(out_values[i]);
    }
    mp_obj_t tuple_values = mp_obj_new_tuple(ml4f_model_example_num_labels, tup_values);

    // And a tuple with the index of the max value and the tuple of predictions
    mp_obj_t tup_max_values[] = { mp_obj_new_int(max_index), tuple_values };
    return mp_obj_new_tuple(2, tup_max_values);
}
static MP_DEFINE_CONST_FUN_OBJ_1(predict_func_obj, predict_func);

// Define all attributes of the module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
static const mp_rom_map_elem_t ml_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ml) },
    { MP_ROM_QSTR(MP_QSTR_cppfunc), MP_ROM_PTR(&cppfunc_obj) },
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
