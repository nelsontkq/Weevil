#!/bin/bash
SRC_DIR=Game
while inotifywait -r -e modify,move,create,delete $SRC_DIR; do
    cmake --build build --target GameModule
done
