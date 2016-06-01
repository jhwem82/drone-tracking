drone-tracking
===
Pirrot Drone based tracking algorithm

Introduction
---

This repo is mainly based on `BebopDecodeStream` project.

Install
---

### Prerequisites

1. Have ARSDK3 installed, [how](http://developer.parrot.com/docs/bebop/#download-all-sources)
2. Have ffmpeg, ncurses and json-c installed (json-c should be renamed to json).

### Install

Simply `make`. `drone-tracking` will be built.

Do this before running `make`
```
export LD_LIBRARY_PATH=./uavControl/lib:$DYLD_LIBRARY_PATH
```

### UAVControl

UAVControl is a independent algorithm library, so you need to sync the submodule `uavControl` and compile it.
Then add the built library path to the system PATH

```
export DYLD_LIBRARY_PATH=./uavControl/lib:$DYLD_LIBRARY_PATH
```
