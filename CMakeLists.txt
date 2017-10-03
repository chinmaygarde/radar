# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

cmake_minimum_required(VERSION 3.5.2)

################################################################################
################################################################################
####################     ______          _               #######################
####################     | ___ \        | |              #######################
####################     | |_/ /__ _  __| | __ _ _ __    #######################
####################     |    // _` |/ _` |/ _` | '__|   #######################
####################     | |\ \ (_| | (_| | (_| | |      #######################
####################     \_| \_\__,_|\__,_|\__,_|_|      #######################
####################                                     #######################
################################################################################
################################################################################

project(Radar)

################################################################################
# Import CMake Utilities
################################################################################


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake")

include(AddCXXWarningIfSupported)
include(AddRecommendedWarningFlags)
include(CheckCompiler)
include(CheckPlatform)
include(HardenToolchain)
include(StandardRadarBench)
include(StandardRadarGraphicsTest)
include(StandardRadarLibrary)
include(StandardRadarTest)
include(SymbolVisibility)
include(EnableLTO)
include(TreatAsCXX)

CheckPlatform()

CheckCompiler()

HardenToolchain()

SetRecommendedSymbolVisibility()

EnableLTO()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

################################################################################
# CCache
################################################################################

find_program(CCACHE_PROGRAM NAMES ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()

################################################################################
# Ranlib Symbol Notifications on Mac
################################################################################

if(APPLE)
  SET(CMAKE_C_ARCHIVE_CREATE
      "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
  SET(CMAKE_CXX_ARCHIVE_CREATE
      "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
  SET(CMAKE_C_ARCHIVE_FINISH
      "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
  SET(CMAKE_CXX_ARCHIVE_FINISH
      "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
endif()

################################################################################
# Benchmarking
################################################################################

if(NOT (APPLE OR LINUX OR WINDOWS))
  set(RADAR_BENCHMARKING_ENABLED OFF CACHE BOOL "Disable benchmarking")
endif()

option(RADAR_BENCHMARKING_ENABLED "Enable benchmarking." ON)

if(RADAR_BENCHMARKING_ENABLED)
  # Build the benchmarking target with its tests disabled.
  enable_testing()
  add_custom_target(bench)
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable tests on benchmarks")
  set(BENCHMARK_ENABLE_EXCEPTIONS OFF CACHE BOOL "Disable exceptions on benchmarks")
  add_subdirectory("ThirdParty/googlebenchmark")
endif()

################################################################################
# Testing
################################################################################

if(NOT (APPLE OR LINUX OR WINDOWS))
  set(RADAR_TESTING_ENABLED OFF CACHE BOOL "Disable testing")
endif()

option(RADAR_TESTING_ENABLED "Enable testing." ON)

if(RADAR_TESTING_ENABLED)
  enable_testing()
endif()

################################################################################
# Graphics Testing
################################################################################

if(NOT (APPLE OR LINUX OR WINDOWS))
  set(RADAR_GRAPHICS_TESTING_ENABLED OFF CACHE BOOL "Disable graphics testing")
endif()

option(RADAR_GRAPHICS_TESTING_ENABLED "Enable graphics testing." ON)

if(RADAR_GRAPHICS_TESTING_ENABLED)
  enable_testing()
endif()

################################################################################
# Sanitizers (Require Custom Clang Setup)
# Usage:
#     $ cmake -DSANITIZE=address
#     Any supported Clang sanitizer can be specified
################################################################################

if(SANITIZE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${SANITIZE} -g -O1")
endif()

################################################################################
# Force in-process variants.
################################################################################

if(DISABLE_XPC)
  add_definitions(-DRL_DISABLE_XPC=1)
endif()

################################################################################
################################################################################
###############     _     _ _                    _               ###############
###############    | |   (_) |                  (_)              ###############
###############    | |    _| |__  _ __ __ _ _ __ _  ___  ___     ###############
###############    | |   | | '_ \| '__/ _` | '__| |/ _ \/ __|    ###############
###############    | |___| | |_) | | | (_| | |  | |  __/\__ \    ###############
###############    \_____/_|_.__/|_|  \__,_|_|  |_|\___||___/    ###############
###############                                                  ###############
################################################################################
################################################################################

add_subdirectory("Library/Animation")
add_subdirectory("Library/BenchmarkRunner")
add_subdirectory("Library/BodymovinParser")
add_subdirectory("Library/Compositor")
add_subdirectory("Library/Coordinator")
add_subdirectory("Library/Core")
add_subdirectory("Library/DevSupport")
add_subdirectory("Library/Entity")
add_subdirectory("Library/Event")
add_subdirectory("Library/Expression")
add_subdirectory("Library/Geometry")
add_subdirectory("Library/GLFoundation")
add_subdirectory("Library/GraphicsTestRunner")
add_subdirectory("Library/Image")
add_subdirectory("Library/Interface")
add_subdirectory("Library/InterfaceBuilder")
add_subdirectory("Library/Layout")
add_subdirectory("Library/Shell")
add_subdirectory("Library/TestRunner")
add_subdirectory("Library/Toolbox")
add_subdirectory("Library/Typography")

add_subdirectory("Samples")

add_subdirectory("ThirdParty/base64")
add_subdirectory("ThirdParty/freetype")
add_subdirectory("ThirdParty/gtest")
add_subdirectory("ThirdParty/harfbuzz")
add_subdirectory("ThirdParty/icu")
add_subdirectory("ThirdParty/imgui")
add_subdirectory("ThirdParty/libtess2")
add_subdirectory("ThirdParty/pugixml")
add_subdirectory("ThirdParty/sqlite3")
add_subdirectory("ThirdParty/stb")
add_subdirectory("ThirdParty/uriparser")

################################################################################
################################################################################
############    ______ _       _    __                             #############
############    | ___ \ |     | |  / _|                            #############
############    | |_/ / | __ _| |_| |_ ___  _ __ _ __ ___  ___     #############
############    |  __/| |/ _` | __|  _/ _ \| '__| '_ ` _ \/ __|    #############
############    | |   | | (_| | |_| || (_) | |  | | | | | \__ \    #############
############    \_|   |_|\__,_|\__|_| \___/|_|  |_| |_| |_|___/    #############
############                                                       #############
################################################################################
################################################################################

################################################################################
# Mac OSX
################################################################################

if(APPLE)
  add_subdirectory("Platforms/Mac")
endif()

################################################################################
# Linux Desktop Shell
################################################################################

if(LINUX)
  add_subdirectory("Platforms/Linux")
endif()

################################################################################
# Rasperry Pi Shell
################################################################################

if(RASPBERRY)
  add_subdirectory("Platforms/RaspberryPi")
endif()

################################################################################
# Native Client (PNaCl Only) Shell
################################################################################

if(NACL)
  add_subdirectory("Platforms/Nacl")
endif()

################################################################################
# Android Shell
################################################################################

if(ANDROID)
  add_subdirectory("Platforms/Android")
endif()

################################################################################
# FreeBSD
################################################################################

# Everything should be automatic. There is no shell, only tests and benchmarks.
