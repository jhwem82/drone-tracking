
LOCAL := ../../local
LOCAL_INCLUDE := $(LOCAL)/include
LOCAL_LIBRARY := $(LOCAL)/lib

EXE := test_ffmpeg

CC := gcc
CXX := g++
CFLAGS := $(shell pkg-config --cflags libavformat libavcodec libswresample libswscale libavutil sdl)
CFLAGS += -Wall

LDFLAGS := $(shell pkg-config --libs libavformat libavcodec libavutil libswscale)

all: dirs $(addprefix bin/, $(EXE))

dirs: 
	@ mkdir -p obj
	@ mkdir -p bin

bin/%: %.o
	@ echo "CC $@"
	@ $(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

obj/%.o: %.c
	@ echo "CC $@"
	@ $(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

.PHONY: clean
clean: 
	rm -rf bin obj
