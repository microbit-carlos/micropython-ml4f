#pragma once

#include <model_example.h>

int get_model_start_address(void);
float* invoke_model(const float *input, const int len);
float* invoke_example_model(const float *input, const int len);
