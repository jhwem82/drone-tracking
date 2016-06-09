
include gmsl

CC := clang
CXX := clang++ -Qunused-arguments
CFLAGS := -Wall

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
LDFLAGS += -lncurses -lopencv_imgproc -lopencv_highgui -lopencv_core

INCLUDE += -Iinclude/

UAV_CONTROL_DIR := ./uavControl
UAV_CONTROL_INC_DIR := $(UAV_CONTROL_DIR)/include
UAV_CONTROL_LIB_DIR := $(UAV_CONTROL_DIR)/lib
UAV_CONTROL_LIB_NAME := UAVControl

INCLUDE += -I$(UAV_CONTROL_INC_DIR)
LDFLAGS += -L$(UAV_CONTROL_LIB_DIR) -l$(UAV_CONTROL_LIB_NAME)

CFLAGS += $(INCLUDE)

EXEC := bebop_track

BIN_DIR := ./bin
OBJ_DIR := ./obj
OBJ := BebopUAVControl.o BebopDroneDecodeStream.o DecoderManager.o ihm.o
OBJS := $(addprefix $(OBJ_DIR)/, $(OBJ))

SRC_DIR := ./src
INC_DIR := ./include
DEPS := $(wildcard $(INC_DIR)/*.h)

all: obj bin $(BIN_DIR)/$(EXEC)

$(BIN_DIR)/$(EXEC): $(OBJS)
	@ echo "CXX\t -o $^ $@"
	@ $(CXX) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@ echo "CC\t -c $< -o $@"
	@ $(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc $(DEPS)
	@ echo "CXX\t -c $< -o $@"
	@ $(CXX) $(CFLAGS) -c $< -o $@ 

obj: 
	@ mkdir -p $(OBJ_DIR)

bin:
	@ mkdir -p $(BIN_DIR)

clean: 
	@ rm -rf $(OBJ_DIR) $(BIN_DIR)
	@ rm -rf *.png *.log *.jpg
