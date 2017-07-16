#!/bin/bash
mkdir -p build
cd build
cmake -DGAWO_BUILD_TESTS=1 -DGAWO_GENERATE_TESTDATA=1 ..
rm tests/*.trace
cmake --build . && cd tests && apitrace trace ./gawo-tests && cd ..
qapitrace tests/gawo-tests.trace
