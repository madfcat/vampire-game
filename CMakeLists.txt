cmake_minimum_required(VERSION 3.22.1)
project(Survive LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES
    src/*.mm
    src/*.m
    src/*.cpp
    src/*.c
    src/*.cxx
    src/*.h
)

add_executable(survive ${SOURCES})
target_link_libraries(survive PRIVATE sfml-graphics sfml-audio sfml-network)
target_compile_features(survive PRIVATE cxx_std_17)

add_custom_command(
    TARGET survive
    COMMENT "Copy assets directory"
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:survive>/assets
    VERBATIM)

if(WIN32)
    add_custom_command(
        TARGET survive
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:survive>
        VERBATIM)
endif()
