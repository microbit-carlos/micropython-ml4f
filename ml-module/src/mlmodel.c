#include <stdint.h>
#include <stddef.h>
#include <ml4f.h>
#include <model_example.h>
#include "mlmodel.h"

// Linker symbols used to find the start of the model in flash.
extern uint8_t  __etext, __data_start__, __data_end__;

// Flag to control usage of model included in model_example.h/c
static bool USE_BUILT_IN = true;

/*****************************************************************************/
/* Private API                                                               */
/*****************************************************************************/
static int get_model_start_address() {
    // The last section in FLASH is meant to be text, but data section contents
    // are placed immediately after it (to be copied to RAM), but there isn't
    // a symbol to indicate its end in FLASH, so we calculate how long data is
    // to figure out the last byte stored in flash.
    int end_of_text_section = (int)(&__etext);
    int data_section_size = (int)(&__data_end__ - &__data_start__);
    int end_of_flash_data = end_of_text_section + data_section_size;

    // TODO: Detect the flash page instead of hardcoding it.
    static const int flash_page_size = 4096;

    // Find the start of the next page after the end of the flash data.
    return (end_of_flash_data + flash_page_size - 1) & ~(flash_page_size - 1);
}

static ml4f_header_t* get_model_header() {
    return (ml4f_header_t *)get_model_start_address();
}

/*****************************************************************************/
/* Public API                                                                */
/*****************************************************************************/
bool use_built_in_model(bool use) {
    USE_BUILT_IN = use;
    return USE_BUILT_IN;
}

bool is_model_present(void) {
    // TODO: Implement built-in module
    return USE_BUILT_IN ? true : false;
}

size_t get_model_label_num(void) {
    // TODO: Implement built-in module
    return USE_BUILT_IN ? ml4f_model_example_num_labels : 0;
}

size_t get_model_input_num() {
    // TODO: Implement built-in module
    return USE_BUILT_IN ? ml4f_model_example_input_num_elements : 0;
}

float* model_predict(const float *input) {
    if (!USE_BUILT_IN) {
        (void)get_model_header();
        return NULL;
    }

    static float out[ml4f_model_example_num_labels];
    int r = ml4f_full_invoke((ml4f_header_t *)ml4f_model_example, input, out);
    if (r != 0) {
        return NULL;
    }
    return (float *)out;
}

size_t calc_arg_max(const float *values, size_t num_elements) {
    return ml4f_argmax(values, (uint32_t)num_elements);
}
