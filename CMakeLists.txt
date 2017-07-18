project(gawo)
cmake_minimum_required(VERSION 3.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modiwlau")
include(LibSetTarget)
include(LibFlexibleBison)

set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/")
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

option(GAWO_BUILD_TESTS "build tests" OFF)
option(USE_X11 "use x11" OFF)
option(USE_SDL "use sdl" ON)
option(COPY_SHADERS "copy the shaders to the binary dir" ON)

if (COPY_SHADERS)
    file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
endif (COPY_SHADERS)
if (GAWO_BUILD_TESTS)
    file(COPY tests/gawo-testdata DESTINATION ${CMAKE_BINARY_DIR})
endif (GAWO_BUILD_TESTS)

find_package(epoxy REQUIRED)
find_package(GLM REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(sig11)

set(ENGINE_SRC
        src/gawo/gl/renderer/camerarenderer.cpp
        src/gawo/gl/wrapper/glclasses.cpp
        src/gawo/gl/wrapper/shader.cpp
        src/gawo/datastructures/scenegraph.cpp
        src/gawo/gl/renderer/spriterenderer.cpp
        src/gawo/io/log.cpp
        )

set(ENGINE_INC
        gawo/gl/renderer/camerarenderer.hpp
        gawo/gl/wrapper/glclasses.hpp
        gawo/gl/wrapper/shader.hpp
        gawo/gl/window/glwindow.hpp
        gawo/datastructures/scenegraph.hpp
        gawo/gl/renderer/spriterenderer.hpp
        gawo/io/log.hpp
        )

#generate parser and lexer for the shader preprocessor

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/spp)
set(LEXER_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/spp/lexer.lpp)
set(PARSER_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/spp/parser.ypp)
set(LEXER_SRC ${CMAKE_CURRENT_BINARY_DIR}/spp/lexer.cpp)
set(PARSER_SRC ${CMAKE_CURRENT_BINARY_DIR}/spp/parser.cpp)
set(PARSER_INC ${CMAKE_CURRENT_BINARY_DIR}/spp/parser.hpp)

set(SPP_SRC
        src/spp/ast.cpp
        src/spp/context.cpp
        src/spp/loader.cpp
        ${LEXER_SRC}
        ${PARSER_SRC}
        )
set(SPP_INC
        spp/ast.hpp
        spp/context.hpp
        spp/lexer.hpp
        spp/loader.hpp
        spp/spp.hpp
        ${PARSER_INC}
        )

generate_lexer(INPUT_FILE ${LEXER_INPUT} OUTPUT_SRC ${LEXER_SRC})
generate_parser(INPUT_FILE ${PARSER_INPUT} OUTPUT_INC ${PARSER_INC} OUTPUT_SRC ${PARSER_SRC})

IF (USE_SDL)
    find_package(SDL2 REQUIRED)
    find_package(SDL2Image REQUIRED)
    find_package(SDL2TTF REQUIRED)
    add_definitions(-DUSE_SDL -DUSE_GAWO)
    set(ENGINE_SRC ${ENGINE_SRC}
            src/gawo/gl/window/sdlwrapper/sdlwindow.cpp)
    set(ENGINE_INC ${ENGINE_INC}
            sdlwrapper/sdlwrapper.hpp
            sdlwrapper/sdlwindow.hpp)
ENDIF (USE_SDL)
IF (USE_X11)
    add_definitions(-DUSE_X11 -DUSE_GAWO)
ENDIF (USE_X11)
setup_cxx_library(TARGET_NAME ${PROJECT_NAME} STATIC ${ENGINE_INC} ${ENGINE_SRC} ${SPP_INC} ${SPP_SRC})
target_link_libraries(${PROJECT_NAME} sig11)
#todo: windows?
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} ${EPOXY_LIBRARIES})
IF (USE_SDL)
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} ${SDL2_TTF_LIBRARIES})
ENDIF (USE_SDL)
IF (USE_X11)
    target_link_libraries(${PROJECT_NAME} x11)
ENDIF (USE_X11)
target_link_libraries(${PROJECT_NAME} "atomic")

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

IF (GAWO_BUILD_TESTS)
    add_subdirectory(tests)
ENDIF (GAWO_BUILD_TESTS)
