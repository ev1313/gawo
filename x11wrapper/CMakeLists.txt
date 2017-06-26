project(x11wrapper)
cmake_minimum_required(VERSION 3.0)

set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/")

find_package(X11 REQUIRED)

set(X11WRAPPER_SRC
  src/x11wrapper/x11window.cpp
  )

set(X11WRAPPER_INC
  x11wrapper/x11window.hpp)

setup_cxx_library(TARGET_NAME ${PROJECT_NAME} STATIC ${X11WRAPPER_INC} ${X11WRAPPER_SRC})

target_link_libraries(${PROJECT_NAME} "atomic")
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
