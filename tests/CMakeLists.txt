set(TEST_SRC
  gawo/scenegraphtest.cpp
  main.cpp
  )

add_executable(tests ${TEST_SRC})
setup_target(tests)

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Catch/include)
target_link_libraries(tests sdlwrapper)
target_link_libraries(tests gawo)
target_link_libraries(tests "atomic")
