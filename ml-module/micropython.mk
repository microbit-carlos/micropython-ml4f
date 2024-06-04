CPPEXAMPLE_MOD_DIR := $(USERMOD_DIR)

SRC_USERMOD += \
	$(CPPEXAMPLE_MOD_DIR)/pxt-ml-runner-poc/mlrunner/ml4f.c \
	$(CPPEXAMPLE_MOD_DIR)/pxt-ml-runner-poc/mlrunner/mlrunner.c \
	$(CPPEXAMPLE_MOD_DIR)/pxt-ml-runner-poc/mlrunner/mldataprocessor.c \
	$(CPPEXAMPLE_MOD_DIR)/pxt-ml-runner-poc/mlrunner/example_dataprocessor.c \
	$(CPPEXAMPLE_MOD_DIR)/src/mlinterface.c \
	$(CPPEXAMPLE_MOD_DIR)/src/mlmodule.c

# SRC_USERMOD_CXX += $(CPPEXAMPLE_MOD_DIR)/ml-runner/mlrunner.cpp

COMMON_FLAGS = \
	-I$(CPPEXAMPLE_MOD_DIR)/src \
	-I$(CPPEXAMPLE_MOD_DIR)/pxt-ml-runner-poc/mlrunner \
	-DDEVICE_MLRUNNER_USE_EXAMPLE_MODEL=2 \
	-DML_DEBUG_PRINT=1

CFLAGS_USERMOD += $(COMMON_FLAGS)

CXXFLAGS_USERMOD += $(COMMON_FLAGS) -std=c++11
