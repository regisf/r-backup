#!/usr/bin/env bash

BUILD_TYPE=Debug

if [ "$1" = "release" ]
then 
	BUILD_TYPE=Release
fi

mkdir -p build
cd build 

conan install .. -s build_type=${BUILD_TYPE} && \
cmake .. && \
cmake --build . --parallel --config ${BUILD_TYPE} && \
bin/r-backup_test
