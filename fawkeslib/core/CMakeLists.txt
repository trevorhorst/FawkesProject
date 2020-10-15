cmake_minimum_required( VERSION 3.5 )

# Define project settings
project( FawkesLibCore
    VERSION 0.0.1
    LANGUAGES CXX C
    DESCRIPTION "Fawkes Project core library"
    )

# Create variable containing locations of source files
set( FAWKES_CORE_INCLUDE_DIR include/fawkes/core )
set( FAWKES_CORE_SOURCE_DIR src )

# Header files to include
set( FAWKES_CORE_INCLUDE ${FAWKES_CORE_INCLUDE}
    ${FAWKES_CORE_INCLUDE_DIR}/json/cjson.h
    ${FAWKES_CORE_INCLUDE_DIR}/logger/log.h
    ${FAWKES_CORE_INCLUDE_DIR}/option_parser.h
    )

# Source files to include
set( FAWKES_CORE_SOURCE ${FAWKES_CORE_SOURCE}
    ${FAWKES_CORE_SOURCE_DIR}/json/cjson.cpp
    ${FAWKES_CORE_SOURCE_DIR}/logger/log.cpp
    )

# Library dependencies
add_library(
    ${PROJECT_NAME}
    SHARED
        ${FAWKES_CORE_INCLUDE}
        ${FAWKES_CORE_SOURCE}
    )

# Alias out the library to include the namespace
add_library(
    FawkesLib::Core ALIAS FawkesLibCore
    )

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        VERSION ${PROJECT_VERSION}
    )

# Specifies include directories to use when compiling a given target
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
