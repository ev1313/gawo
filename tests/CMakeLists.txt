set(TEST_SRC
  gawo/scenegraphtest.cpp
  main.cpp
  )

setup_cxx_executable(TARGET_NAME tests ${TEST_SRC})

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Catch/include)
target_link_libraries(tests gawo)
target_link_libraries(tests "atomic")
