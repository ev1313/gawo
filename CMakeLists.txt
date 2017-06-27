project(gawo)
cmake_minimum_required(VERSION 3.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modiwlau")
include(LibSetTarget)

set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/")
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

option(CXX_VERSION_14 "C++ Version 14" ON)
option(BUILD_TESTS "build tests" OFF)
option(USE_X11 "use x11" OFF)
option(USE_SDL "use sdl" ON)

find_package(epoxy REQUIRED)
find_package(GLM REQUIRED)

add_subdirectory(sig11)

set(ENGINE_SRC
  src/gawo/camerarenderer.cpp
  src/gawo/glclasses.cpp
  src/gawo/scenegraph.cpp
  src/gawo/spriterenderer.cpp
  )

set(ENGINE_INC
  gawo/camerarenderer.hpp
  gawo/glclasses.hpp
  gawo/glwindow.hpp
  gawo/scenegraph.hpp
  gawo/spriterenderer.hpp
  )

IF(USE_SDL)
  add_definitions(-DUSE_SDL -DUSE_GAWO)
  set(ENGINE_SRC ${ENGINE_SRC} 
	  src/sdlwrapper/sdlwindow.cpp)
  set(ENGINE_INC ${ENGINE_INC}
	  sdlwrapper/sdlwrapper.hpp
	  sdlwrapper/sdlwindow.hpp)
ENDIF(USE_SDL)
IF(USE_X11)
  add_definitions(-DUSE_X11 -DUSE_GAWO)
ENDIF(USE_X11)
setup_cxx_library(TARGET_NAME ${PROJECT_NAME} STATIC ${ENGINE_INC} ${ENGINE_SRC}) 
target_link_libraries(${PROJECT_NAME} sig11)
target_link_libraries(${PROJECT_NAME} ${EPOXY_LIBRARIES})
IF(USE_SDL)
ENDIF(USE_SDL)
IF(USE_X11)
ENDIF(USE_X11)
target_link_libraries(${PROJECT_NAME} "atomic")

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

IF(BUILD_TESTS)
  add_subdirectory(tests)
ENDIF(BUILD_TESTS)
