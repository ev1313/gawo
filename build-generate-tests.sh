#!/bin/bash
mkdir -p build
cd build
cmake -DGAWO_BUILD_TESTS=1 -DGAWO_GENERATE_TESTDATA=1 ..
cmake --build . && cd tests && ./gawo-tests && cd ..
