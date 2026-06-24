#!/bin/bash

set -e

echo "Start building process"

if [ ! -d "build" ];then
	echo "Creating directory 'build'..."
	mkdir build
fi

cd build

cmake ..

make -j$(nproc)


