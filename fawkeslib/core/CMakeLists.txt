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
    ${FAWKES_CORE_INCLUDE_DIR}/control/control.h
    ${FAWKES_CORE_INCLUDE_DIR}/control/control_template.h
    ${FAWKES_CORE_INCLUDE_DIR}/error/error.h
    ${FAWKES_CORE_INCLUDE_DIR}/json/cjson.h
    ${FAWKES_CORE_INCLUDE_DIR}/logger/log.h
    ${FAWKES_CORE_INCLUDE_DIR}/network/unix/unix_client.h
    ${FAWKES_CORE_INCLUDE_DIR}/network/unix/unix_server.h
    ${FAWKES_CORE_INCLUDE_DIR}/option_parser.h
    ${FAWKES_CORE_INCLUDE_DIR}/transport/client.h
    ${FAWKES_CORE_INCLUDE_DIR}/transport/server.h
    )

# Source files to include
set( FAWKES_CORE_SOURCE ${FAWKES_CORE_SOURCE}
    ${FAWKES_CORE_SOURCE_DIR}/control/control.cpp
    ${FAWKES_CORE_SOURCE_DIR}/error/error.cpp
    ${FAWKES_CORE_SOURCE_DIR}/json/cjson.cpp
    ${FAWKES_CORE_SOURCE_DIR}/logger/log.cpp
    ${FAWKES_CORE_SOURCE_DIR}/network/unix/unix_client.cpp
    ${FAWKES_CORE_SOURCE_DIR}/network/unix/unix_server.cpp
    ${FAWKES_CORE_SOURCE_DIR}/transport/client.cpp
    ${FAWKES_CORE_SOURCE_DIR}/transport/server.cpp
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
