cmake_minimum_required(VERSION 3.5)
project (BasicRT3 VERSION 1.0.0 LANGUAGES CXX )

#=== CREATING COMPILING DATABASE ===#
# Currently CMake (since 2.8.5) supports generation of compilation databases
# for Unix Makefile builds (Ninja builds in the works) with the option
# CMAKE_EXPORT_COMPILE_COMMANDS.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#=== FINDING PACKAGES ===#

# # find_package(TinyXml2 REQUIRED)

# Set "manually" paths that need to be considered while compiling/linking
include_directories( cameras
                     materials
                     lights
                     integrators
                     core
                     ext )

#=== SETTING VARIABLES ===#
# Compiling flags
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -pedantic -O3 -ffast-math" )
# set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -pedantic" )
set( RT3_SOURCE_DIR "src" )

#=== main  target ===
file( GLOB SOURCE_BASICRT3 ${RT3_SOURCE_DIR}/cameras/*.cpp
                           ${RT3_SOURCE_DIR}/shapes/*.cpp
                           ${RT3_SOURCE_DIR}/lights/*.cpp
                           ${RT3_SOURCE_DIR}/integrators/*.cpp
                           ${RT3_SOURCE_DIR}/materials/*.cpp
                           ${RT3_SOURCE_DIR}/lights/*.cpp
                           ${RT3_SOURCE_DIR}/core/*.cpp
                           ${RT3_SOURCE_DIR}/ext/*.cpp
                           ${RT3_SOURCE_DIR}/ext/*.cc
                           ${RT3_SOURCE_DIR}/main/*.cpp
                        )
add_executable(basic_rt3 ${SOURCE_BASICRT3})

target_link_libraries(basic_rt3)

#define C++17 as the standard.
set_property(TARGET basic_rt3 PROPERTY CXX_STANDARD 17)
