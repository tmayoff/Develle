#!/bin/bash

currentDir=$PWD
cd subprojects/tracy/profiler/build/unix
make
cd $currentDir
./subprojects/tracy/profiler/build/unix/Tracy-release
