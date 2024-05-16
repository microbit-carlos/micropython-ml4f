CPPEXAMPLE_MOD_DIR := $(USERMOD_DIR)

SRC_USERMOD += \
	$(CPPEXAMPLE_MOD_DIR)/model-example/model_example.c \
	$(CPPEXAMPLE_MOD_DIR)/mllib/ml4f/ml4f.c \
	$(CPPEXAMPLE_MOD_DIR)/mllib/mllib.c \
	$(CPPEXAMPLE_MOD_DIR)/src/mlmodule.c

CFLAGS_USERMOD += \
	-I$(CPPEXAMPLE_MOD_DIR)/mllib \
	-I$(CPPEXAMPLE_MOD_DIR)/model-example \
	-I$(CPPEXAMPLE_MOD_DIR)/src
