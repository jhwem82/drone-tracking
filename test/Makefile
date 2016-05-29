
LOCAL := ../../local
LOCAL_INCLUDE := $(LOCAL)/include
LOCAL_LIBRARY := $(LOCAL)/lib

EXE := test_ffmpeg

CC := gcc
CXX := g++
CFLAGS := $(shell pkg-config --cflags libavformat libavcodec libswresample libswscale libavutil sdl)
CFLAGS += -Wall -Qunused-arguments

LDFLAGS := $(shell pkg-config --libs libavformat libavcodec libavutil libswscale) -lopencv_highgui -lopencv_imgproc -lopencv_core

all: dirs $(addprefix bin/, $(EXE))

bin/%: obj/%.o
	@ echo "CXX $^ -o $@"
	@ $(CXX) $(CFLAGS) $< -o $@ $(LDFLAGS)

obj/%.o: %.cc
	@ echo "CXX -c $< -o $@"
	@ $(CXX) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

.PHONY: dirs clean

dirs: 
	@ mkdir -p obj
	@ mkdir -p bin

clean: 
	rm -rf bin obj
