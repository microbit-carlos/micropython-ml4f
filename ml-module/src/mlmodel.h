#pragma once

#include <stdbool.h>

bool use_built_in_model(bool use);
bool is_model_present(void);
size_t get_model_label_num(void);
size_t get_model_input_num(void);
float* model_predict(const float *input);
size_t calc_arg_max(const float *values, size_t num_elements);
