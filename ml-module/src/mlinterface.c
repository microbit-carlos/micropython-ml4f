#include "mlinterface.h"

#if DEVICE_MLRUNNER_USE_EXAMPLE_MODEL == 1
    #include "example_model1.h"
#elif DEVICE_MLRUNNER_USE_EXAMPLE_MODEL == 2
    #include "example_model2.h"
#else
    #define DEVICE_MLRUNNER_USE_EXAMPLE_MODEL 0
#endif

// Linker symbols used to find the start of the model in flash.
extern uint8_t  __etext, __data_start__, __data_end__;


/**
 * @brief Get the start address of the full model (header + ML4F model).
 *
 * This would also be the start address of the model header with the labels.
 * This function does not check if the data in flash is a valid model.
 *
 * @return The start address to where the full model is stored in flash.
 */
#if DEVICE_MLRUNNER_USE_EXAMPLE_MODEL == 0
static uint32_t get_model_flash_address() {
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
#endif


static void* get_model_address() {
#if DEVICE_MLRUNNER_USE_EXAMPLE_MODEL > 0
    return (void *)example_model;
#else
    return (void *)get_model_flash_address();
#endif
}


bool ml_loadModel() {
    return ml_setModel(get_model_address());
}
