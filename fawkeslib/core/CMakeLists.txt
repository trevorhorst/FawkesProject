cmake_minimum_required( VERSION 3.5 )

# Define project settings
project( FawkesLibCore
    VERSION 0.0.1
    LANGUAGES CXX C
    DESCRIPTION "Fawkes Project core library"
    )

# Creates a portable way to implement the __FILENAME__ macro
# https://stackoverflow.com/questions/1706346/file-macro-manipulation-handling-at-compile-time
function(define_file_basename_for_sources targetname)
    get_target_property(source_files "${targetname}" SOURCES)
    foreach(sourcefile ${source_files})
        # Add the FILE_BASENAME=filename compile definition to the list.
        get_filename_component(basename "${sourcefile}" NAME)
        # Set the updated compile definitions on the source file.
        set_property(
            SOURCE "${sourcefile}" APPEND
            PROPERTY COMPILE_DEFINITIONS "__FILENAME__=\"${basename}\"")
    endforeach()
endfunction()

# Create variable containing locations of source files
set( FAWKES_CORE_INCLUDE_DIR include/fawkes/core )
set( FAWKES_CORE_SOURCE_DIR src )

# Header files to include
set( FAWKES_CORE_INCLUDE ${FAWKES_CORE_INCLUDE}
    ${FAWKES_CORE_INCLUDE_DIR}/command/command.h
    ${FAWKES_CORE_INCLUDE_DIR}/common_types.h
    ${FAWKES_CORE_INCLUDE_DIR}/command/command_debug.h
    ${FAWKES_CORE_INCLUDE_DIR}/command/command_handler.h
    ${FAWKES_CORE_INCLUDE_DIR}/command/command_template.h
    ${FAWKES_CORE_INCLUDE_DIR}/command/command_test.h
    ${FAWKES_CORE_INCLUDE_DIR}/control/control.h
    ${FAWKES_CORE_INCLUDE_DIR}/control/control_template.h
    ${FAWKES_CORE_INCLUDE_DIR}/debug.h
    ${FAWKES_CORE_INCLUDE_DIR}/error/error.h
    ${FAWKES_CORE_INCLUDE_DIR}/incbin.h
    ${FAWKES_CORE_INCLUDE_DIR}/json/cjson.h
    ${FAWKES_CORE_INCLUDE_DIR}/logger/log.h
    ${FAWKES_CORE_INCLUDE_DIR}/network/unix/unix_client.h
    ${FAWKES_CORE_INCLUDE_DIR}/network/unix/unix_server.h
    ${FAWKES_CORE_INCLUDE_DIR}/option_parser.h
    ${FAWKES_CORE_INCLUDE_DIR}/resource.h
    ${FAWKES_CORE_INCLUDE_DIR}/singleton.h
    ${FAWKES_CORE_INCLUDE_DIR}/transport/client.h
    ${FAWKES_CORE_INCLUDE_DIR}/transport/server.h
    ${FAWKES_CORE_INCLUDE_DIR}/types/byte_array.h
    )

# Source files to include
set( FAWKES_CORE_SOURCE ${FAWKES_CORE_SOURCE}
    ${FAWKES_CORE_SOURCE_DIR}/command/command.cpp
    ${FAWKES_CORE_SOURCE_DIR}/command/command_debug.cpp
    ${FAWKES_CORE_SOURCE_DIR}/command/command_handler.cpp
    ${FAWKES_CORE_SOURCE_DIR}/command/command_test.cpp
    ${FAWKES_CORE_SOURCE_DIR}/common_types.cpp
    ${FAWKES_CORE_SOURCE_DIR}/control/control.cpp
    ${FAWKES_CORE_SOURCE_DIR}/debug.cpp
    ${FAWKES_CORE_SOURCE_DIR}/error/error.cpp
    ${FAWKES_CORE_SOURCE_DIR}/json/cjson.cpp
    ${FAWKES_CORE_SOURCE_DIR}/logger/log.cpp
    ${FAWKES_CORE_SOURCE_DIR}/network/unix/unix_client.cpp
    ${FAWKES_CORE_SOURCE_DIR}/network/unix/unix_server.cpp
    ${FAWKES_CORE_SOURCE_DIR}/resource.cpp
    ${FAWKES_CORE_SOURCE_DIR}/transport/client.cpp
    ${FAWKES_CORE_SOURCE_DIR}/transport/server.cpp
    ${FAWKES_CORE_SOURCE_DIR}/types/byte_array.cpp
    )

# Library dependencies
add_library(
    ${PROJECT_NAME}
    SHARED
        ${FAWKES_CORE_INCLUDE}
        ${FAWKES_CORE_SOURCE}
    )

# Create the __FILENAME__ macro for use with the logger
define_file_basename_for_sources(${PROJECT_NAME})

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
