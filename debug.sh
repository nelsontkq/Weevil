#!/bin/bash

CMAKE_FLAGS="-DWV_ENABLE_ASSERT=ON -DWV_LOG_LEVEL=4 -DCMAKE_BUILD_TYPE=Debug"
BUILD_DIR=build
cmake -S . $CMAKE_FLAGS -B $BUILD_DIR
cmake --build $BUILD_DIR

SRC_DIR=Game
while inotifywait -r -e modify,move,create,delete $SRC_DIR; do
    cmake --build $BUILD_DIR --target GameModule
done
