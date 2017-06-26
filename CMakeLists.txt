project(gawo)
cmake_minimum_required(VERSION 3.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modiwlau")
include(LibSetTarget)

set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/")


option(CXX_VERSION_14 "C++ Version 14" ON)
option(BUILD_TESTS "build tests" OFF)
option(USE_X11 "use x11" OFF)
option(USE_SDL "use sdl" ON)

IF(USE_SDL)
  add_subdirectory(sdlwrapper)
ENDIF(USE_SDL)
IF(USE_X11)
  add_subdirectory(x11wrapper)
ENDIF(USE_X11)
add_subdirectory(sig11)

find_package(epoxy REQUIRED)
find_package(GLM REQUIRED)

set(ENGINE_SRC
  src/gawo/camerarenderer.cpp
  src/gawo/glclasses.cpp
  src/gawo/scenegraph.cpp
  src/gawo/spriterenderer.cpp
  )

set(ENGINE_INC
  gawo/camerarenderer.hpp
  gawo/glclasses.hpp
  gawo/scenegraph.hpp
  gawo/spriterenderer.hpp
  )

setup_cxx_library(TARGET_NAME ${PROJECT_NAME} STATIC ${ENGINE_INC} ${ENGINE_SRC})

target_link_libraries(${PROJECT_NAME} ${EPOXY_LIBRARIES})
IF(USE_SDL)
  target_link_libraries(${PROJECT_NAME} sdlwrapper)
ENDIF(USE_SDL)
IF(USE_X11)
  target_link_libraries(${PROJECT_NAME} x11wrapper)
ENDIF(USE_X11)
target_link_libraries(${PROJECT_NAME} "atomic")

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

IF(BUILD_TESTS)
  add_subdirectory(tests)
ENDIF(BUILD_TESTS)
