#pragma once

#include <stdbool.h>
#include <mlrunner.h>

// Enable/disable debug print to serial, can be set in pxt.json
#ifndef ML_DEBUG_PRINT
    #define ML_DEBUG_PRINT 0
#endif
#if ML_DEBUG_PRINT
    #include <py/runtime.h>
    #define DEBUG_PRINT(...) mp_printf(&mp_plat_print, __VA_ARGS__)
#else
    #define DEBUG_PRINT(...)
#endif

bool ml_loadModel(void);
