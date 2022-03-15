#!/bin/bash

if [ -z $1 ]
	then
		echo "Usage: $0 BUILD_TYPE"
		exit 1
fi

mkdir -p build
cd build
mkdir -p $1
cd $1

echo "Configuring project..."
cmake -DCMAKE_TOOLCHAIN_FILE=../../toolchain-i386.cmake -DCMAKE_BUILD_TYPE=$1 ../..

if [ $? -gt 0 ]
	then
		exit $?
fi

echo "Building project..."
make install

if [ $? -gt 0 ]
	then
		exit $?
fi