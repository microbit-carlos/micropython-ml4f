#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
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

ml_prediction_t* model_predict(const float *input) {
    if (!USE_BUILT_IN) {
        (void)get_model_header();
        return NULL;
    }

    typedef struct out_s {
        size_t len;
        float* values;
    } out_t;

    static out_t out = {
        .len = 0,
        .values = NULL
    };
    static ml_prediction_t predictions = {
        .max_index = 0,
        .num_labels = ml4f_model_example_num_labels,
        .predictions = {
            { .prediction = 0.0, .label = "Jumping" },
            { .prediction = 0.0, .label = "Running" },
            { .prediction = 0.0, .label = "Standing" },
            { .prediction = 0.0, .label = "Walking" },
        }
    };

    size_t label_num = get_model_label_num();

    // The model shouldn't really change (only during testing while we built-in
    // one), so this should be a one-time allocation.
    if (out.len != label_num || out.values == NULL) {
        if (out.values != NULL) {
            free(out.values);
        }
        out.len = label_num;
        out.values = (float *)malloc(out.len * sizeof(float));
    }

    int r = ml4f_full_invoke((ml4f_header_t *)ml4f_model_example, input, out.values);
    if (r != 0) {
        return NULL;
    }

    for (int i = 0; i < out.len; i++) {
        predictions.predictions[i].prediction = out.values[i];
    }
    predictions.max_index = ml4f_argmax(out.values, out.len);

    return &predictions;
}
