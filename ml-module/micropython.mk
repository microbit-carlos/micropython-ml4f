CPPEXAMPLE_MOD_DIR := $(USERMOD_DIR)

SRC_USERMOD += \
	$(CPPEXAMPLE_MOD_DIR)/ml-runner/model-example/model_example.c \
	$(CPPEXAMPLE_MOD_DIR)/ml-runner/ml4f/ml4f.c \
	$(CPPEXAMPLE_MOD_DIR)/src/mlmodule.c

SRC_USERMOD_CXX += $(CPPEXAMPLE_MOD_DIR)/ml-runner/mlrunner.cpp

CFLAGS_USERMOD += -I$(CPPEXAMPLE_MOD_DIR)/ml-runner

CXXFLAGS_USERMOD += -I$(CPPEXAMPLE_MOD_DIR)/ml-runner -std=c++11
