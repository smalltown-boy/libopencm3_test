PROJECT = awesomesauce
BUILD_DIR = bin

SHARED_DIR = ../my-common-code
CXXFILES = my-project.cxx
CXXFILES += usart.cxx
CXXFILES += adc.cxx
CXXFILES += key.cxx
CXXFILES += beep.cxx
CFILES += api.c
AFILES += api-asm.S

# TODO - you will need to edit these two lines!
DEVICE=stm32f407vet6					
OOCD_FILE = board/stm32f4discovery.cfg

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

LDLIBS += -lm

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
