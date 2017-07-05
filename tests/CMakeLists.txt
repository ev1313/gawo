add_subdirectory(glits)

set(TEST_SRC
  gawo/scenegraphtest.cpp
  main.cpp
  )

setup_cxx_executable(TARGET_NAME gawo-tests ${TEST_SRC})

target_include_directories(gawo-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Catch/include)
target_link_libraries(gawo-tests gawo)
target_link_libraries(gawo-tests glits)
target_link_libraries(gawo-tests "atomic")
