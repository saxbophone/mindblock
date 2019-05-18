# begin basic metadata
cmake_minimum_required(VERSION 3.0)

project(mindblock VERSION 0.0.0 LANGUAGES CXX)
# set build number - this increments for any new version
set(mindblock_VERSION_BUILD 1)

set(
    mindblock_VERSION_STRING
    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)
set(mindblock_ESCAPED_VERSION_STRING "\"${mindblock_VERSION_STRING}\"")
# end basic metadata

# pass in version of program as preprocessor definitions
add_definitions(-Dmindblock_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})
add_definitions(-Dmindblock_VERSION_MINOR=${PROJECT_VERSION_MINOR})
add_definitions(-Dmindblock_VERSION_PATCH=${PROJECT_VERSION_PATCH})
add_definitions(-Dmindblock_VERSION_BUILD=${mindblock_VERSION_BUILD})
add_definitions(-Dmindblock_VERSION_STRING=${mindblock_ESCAPED_VERSION_STRING})

message(
    STATUS
    "[mindblock] Preparing to build mindblock v${mindblock_VERSION_STRING} "
    "(build ${mindblock_VERSION_BUILD})"
)

# set default C++ standard to use (C++11)
set(mindblock_CXX_STANDARD "11")
# if env variable mindblock_CXX_STANDARD is set, override version
if(DEFINED ENV{mindblock_CXX_STANDARD})
    set(mindblock_CXX_STANDARD "$ENV{mindblock_CXX_STANDARD}")
endif()
message(STATUS "[mindblock] C++ Standard set to C++${mindblock_CXX_STANDARD}")
set(CMAKE_CXX_STANDARD ${mindblock_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# used for enabling additional compiler options if supported
include(CheckCXXCompilerFlag)

function(enable_cxx_compiler_flag_if_supported flag)
    message(STATUS "[mindblock] Checking if compiler supports warning flag '${flag}'")
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            message(STATUS "[mindblock] Enabling warning flag '${flag}'")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
    endif()
    unset(flag_already_set CACHE)
    unset(flag_supported CACHE)
endfunction()

# enable extra flags (warnings) if we're not in release mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "[mindblock] Warnings Enabled")
    # enable all warnings about 'questionable constructs'
    enable_cxx_compiler_flag_if_supported("-Wall")
    # issue 'pedantic' warnings for strict ISO compliance
    enable_cxx_compiler_flag_if_supported("-pedantic")
    # enable 'extra' strict warnings
    enable_cxx_compiler_flag_if_supported("-Wextra")
    # convert all warnings into errors
    enable_cxx_compiler_flag_if_supported("-Werror")
endif()

# default build type is only static if on Windows
if(WIN32)
    set(mindblock_STATIC_BUILD ON CACHE BOOL "")
else()
    set(mindblock_STATIC_BUILD OFF CACHE BOOL "")
endif()

# do necessary things if static build was requested
if(mindblock_STATIC_BUILD)
    message(STATUS "[mindblock] Performing a Static Build")
    set(SFML_STATIC_LIBRARIES ON)
endif()

# unset BUILD_SHARED_LIBS, in case it was set
# this affects whether *we* build as static or shared
# in this instance, we always want to build libmindblock as a static library as
# we're not going to install it separately at all and want to bundle it with
# all executables that depend on it
unset(BUILD_SHARED_LIBS)

# dependencies
# add custom dependencies directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# SDL2
find_package(SFML 2 REQUIRED COMPONENTS audio graphics window system)
include_directories(${SFML_INCLUDE_DIR})
# COLRCV
find_package(COLRCV 0.1 REQUIRED)
include_directories(${COLRCV_INCLUDE_DIR})

# compile the bulk of mindblock into a library (libmindblock)
# this is then shared among the programs which depend on it

# libmindblock C++ Sources
file(GLOB LIBmindblock_SOURCES "mindblock/*.cpp")
# libmindblock Headers
file(GLOB LIBmindblock_HEADERS "mindblock/*.hpp")

# here we define the libmindblock library
add_library(mindblock ${LIBmindblock_SOURCES})

# if we're on windows, then set these additional linker options so the console
# window doesn't show up and get in the way
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
endif()

# here we link libmindblock with its dependencies
# we need to do additional linking steps if it's a static build
if(mindblock_STATIC_BUILD)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # if we're using GCC then these are the flags we need to use to enable
        # statically linking the C and C++ standard libraries
        #
        # this is useful if it's unlikely a recipient of the software will
        # have the correct versions of these on their system (e.g. MinGW
        # compiling for Windows)
        #
        # This is allowed as per the GCC Runtime Exception.
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
    endif()
    # if we're statically linking, then we need to manually link SFML's dependencies as well
    target_link_libraries(mindblock ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${COLRCV_LIBRARIES})
else()
    # if we're dynamically linking, we only need to link SFML and COLRCV
    target_link_libraries(mindblock ${SFML_LIBRARIES} ${COLRCV_LIBRARIES})
endif()

# specify executables and link with the library
# this is the cross-platform demo program
add_executable(mindblock-demo "demo.cpp")
target_link_libraries(mindblock-demo mindblock)

install(TARGETS mindblock-demo DESTINATION bin)
