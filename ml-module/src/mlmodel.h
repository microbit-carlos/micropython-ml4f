#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <cmsis_compiler.h>
#include <ml4f.h>

// ASCII for "MLAB"
#define MODEL_LABELS_MAGIC0 0x4D444C42

typedef __PACKED_STRUCT ml_model_header_t {
    uint32_t magic0;
    uint16_t header_size;      // Size of this header + all label strings
    uint16_t model_offset;     // header_size + padding to 4 bytes
    uint8_t number_of_labels;  // Only 255 labels supported
    uint8_t reserved[7];
    char labels[];             // Mutiple null-terminated strings, as many as number_of_labels
} ml_model_header_t;

typedef struct ml_labels_s {
    size_t num_labels;
    const char **labels;
} ml_labels_t;

typedef struct ml_prediction_s {
    size_t max_index;
    size_t num_labels;
    const char **labels;
    float *predictions;
} ml_prediction_t;

bool get_use_built_in_model(void);
void set_use_built_in_model(bool use);
bool is_model_present(void);
size_t get_model_label_num(void);
ml_labels_t* get_model_labels(void);
size_t get_model_input_num(void);
ml_prediction_t* model_predict(const float *input);
