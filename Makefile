
include gmsl

CC := gcc
CCFLAGS := -Wall

LIBRARIES := \
	ARSAL \
	ARCommands \
	ARNetwork \
	ARNetworkAL \
	ARDiscovery \
	ARStream 

LIBRARY_NAMES := $(addprefix lib, $(LIBRARIES))

# SDK_DIR should be configured
SDK_DIR := ../sdk
BUILD_DIR := $(SDK_DIR)/out/arsdk-native/build
SRC_DIR := $(SDK_DIR)/packages
INCLUDE := $(foreach lib, $(LIBRARY_NAMES), -I$(SRC_DIR)/$(lib)/Includes -I$(SRC_DIR)/$(lib)/gen/Includes)
LDFLAGS := $(foreach lib, $(LIBRARIES), -L$(BUILD_DIR)/lib$(lib) -l$(call lc,$(lib)))

# LOCAL should be configured
LOCAL := ../local
LOCAL_INCLUDE := $(LOCAL)/include
LOCAL_LIBRARY := $(LOCAL)/lib
INCLUDE += $(shell pkg-config --cflags libavformat libavcodec libswresample libswscale libavutil sdl json)
LDFLAGS += $(shell pkg-config --libs libavformat libavcodec libavutil libswscale json)
LDFLAGS += -lncurses

INCLUDE += -Iinclude/

CCFLAGS += $(INCLUDE)

EXEC := bebop_track

BIN_DIR := ./bin
OBJ_DIR := ./obj
OBJ := BebopDroneDecodeStream.o DecoderManager.o ihm.o
OBJS := $(addprefix $(OBJ_DIR)/, $(OBJ))

SRC_DIR := ./src
INC_DIR := ./include
DEPS := $(wildcard $(INC_DIR)/*.h)

all: obj bin $(BIN_DIR)/$(EXEC)

$(BIN_DIR)/$(EXEC): $(OBJS)
	@ echo "CC $^ -o $@"
	@ $(CC) $(CCFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@ echo "CC -c $< -o $@"
	@ $(CC) $(CCFLAGS) -c $< -o $@ 

obj: 
	@ mkdir -p $(OBJ_DIR)

bin:
	@ mkdir -p $(BIN_DIR)

clean: 
	@ rm -rf $(OBJ_DIR) $(BIN_DIR)
