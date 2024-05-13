#include <stdint.h>
#include <stddef.h>
#include <ml4f.h>
#include "mlmodel.h"


// Linker symbols
extern uint8_t  __etext, __data_start__, __data_end__;

int get_model_start_address() {
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

ml4f_header_t* get_model_header() {
    return (ml4f_header_t *)get_model_start_address();
}

float* invoke_model(const float *input, const int len) {
    static float out[ml4f_model_example_num_labels];
    int r = ml4f_full_invoke(get_model_header(), input, out);
    if (r != 0) {
        return NULL;
    }
    return (float *)out;
}

float* invoke_example_model(const float *input, const int len) {
    static float out[ml4f_model_example_num_labels];
    int r = ml4f_full_invoke((ml4f_header_t *)ml4f_model_example, input, out);
    if (r != 0) {
        return NULL;
    }
    return (float *)out;
}