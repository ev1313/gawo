add_subdirectory(glits)

set(TEST_SRC
  gawo/scenegraphtest.cpp
  gawo/spriterenderertest.cpp
  main.cpp
  )

option(GAWO_GENERATE_TESTDATA "generate testdata" OFF)

if(GAWO_GENERATE_TESTDATA)
	add_definitions(-DGAWO_GENERATE_TESTDATA=1)
else(GAWO_GENERATE_TESTDATA)
	add_definitions(-DGAWO_GENERATE_TESTDATA=0)
endif(GAWO_GENERATE_TESTDATA)

setup_cxx_executable(TARGET_NAME gawo-tests ${TEST_SRC})

target_include_directories(gawo-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Catch/include)
target_link_libraries(gawo-tests gawo)
target_link_libraries(gawo-tests glits)
target_link_libraries(gawo-tests "atomic")
