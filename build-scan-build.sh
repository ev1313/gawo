#!/bin/bash
mkdir -p build
cd build
scan-build cmake -DGAWO_BUILD_TESTS=1 -DGAWO_GENERATE_TESTDATA=0 ..
scan-build cmake --build . && cd tests && ./gawo-tests && cd ..
