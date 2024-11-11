cmake_minimum_required(VERSION 3.22.1)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)


# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type: Debug, Release" FORCE)
endif()

# Add flags based on build type
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
    add_compile_options(-g -fsanitize=address)  # or other debug flags
	# Ensure you're using gcc or clang
	if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		set(SANITIZE_FLAGS "-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZE_FLAGS}")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZE_FLAGS}")
	endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode")
    add_compile_options(-O3)  # Optimization flags for release
endif()


include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
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

target_link_libraries(survive PRIVATE sfml-graphics sfml-audio sfml-window sfml-system)
target_compile_features(survive PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET survive
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()



# cmake_minimum_required(VERSION 3.22.1)
# project(Survive LANGUAGES CXX)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# include(FetchContent)
# FetchContent_Declare(SFML
#     GIT_REPOSITORY https://github.com/SFML/SFML.git
#     GIT_TAG 2.6.x
#     GIT_SHALLOW ON)
# FetchContent_MakeAvailable(SFML)

# file(GLOB_RECURSE SOURCES
#     src/*.mm
#     src/*.m
#     src/*.cpp
#     src/*.c
#     src/*.cxx
#     src/*.h
# )

# add_executable(survive ${SOURCES})
# target_link_libraries(survive PRIVATE sfml-graphics sfml-audio sfml-network)
# target_compile_features(survive PRIVATE cxx_std_17)

# add_custom_command(
#     TARGET survive
#     COMMENT "Copy assets directory"
#     POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:survive>/assets
#     VERBATIM)

# if(WIN32)
#     add_custom_command(
#         TARGET survive
#         COMMENT "Copy OpenAL DLL"
#         PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:survive>
#         VERBATIM)
# endif()
