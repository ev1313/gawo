project(gawo)
cmake_minimum_required(VERSION 3.0)

set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

option(CXX_VERSION "C++ Version" 14)
option(BUILD_TESTS "build tests" OFF)
option(USE_X11 "use x11" OFF)
option(USE_SDL "use sdl" ON)

add_subdirectory(sdlwrapper)
add_subdirectory(sig11)

find_package(epoxy REQUIRED)
find_package(GLM REQUIRED)

function(setup_target NAME)
  set_property(TARGET ${NAME} PROPERTY CXX_STANDARD CXX_VERSION)
  set_property(TARGET ${NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
  target_compile_options(${NAME} PRIVATE -Wall -Wextra)
  target_compile_options(${NAME} PRIVATE $<$<CONFIG:DEBUG>:-ggdb -O2>)
  target_compile_options(${NAME} PRIVATE $<$<CONFIG:RELEASE>:-O3 -NDEBUG>)
endfunction(setup_target)

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

add_library(${PROJECT_NAME} STATIC ${ENGINE_INC} ${ENGINE_SRC})
setup_target(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} ${EPOXY_LIBRARIES})
target_link_libraries(${PROJECT_NAME} sdlwrapper)
target_link_libraries(${PROJECT_NAME} "atomic")

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

IF(BUILD_TESTS)
  add_subdirectory(tests)
ENDIF(BUILD_TESTS)
