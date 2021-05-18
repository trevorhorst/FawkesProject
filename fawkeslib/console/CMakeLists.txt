cmake_minimum_required( VERSION 3.5 )

project( FawkesLibConsole
    VERSION 0.0.1
    LANGUAGES CXX C
    DESCRIPTION "Fawkes Project console library"
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
set( FAWKES_CONSOLE_INCLUDE_DIR include/fawkes/console )
set( FAWKES_CONSOLE_SOURCE_DIR src )

set( FAWKES_CONSOLE_INCLUDE ${FAWKES_CONSOLE_INCLUDE}
    ${FAWKES_CONSOLE_INCLUDE_DIR}/console.h
    ${FAWKES_CONSOLE_INCLUDE_DIR}/command_console.h
    ${FAWKES_CONSOLE_INCLUDE_DIR}/command_help.h
    )

set( FAWKES_CONSOLE_SOURCE ${FAWKES_CONSOLE_SOURCE}
    ${FAWKES_CONSOLE_SOURCE_DIR}/console.cpp
    ${FAWKES_CONSOLE_SOURCE_DIR}/command_console.cpp
    ${FAWKES_CONSOLE_SOURCE_DIR}/command_help.cpp
    )

add_library(
    ${PROJECT_NAME}
    SHARED
        ${FAWKES_CONSOLE_INCLUDE}
        ${FAWKES_CONSOLE_SOURCE}
    )

# Alias out the library to include the namespace
add_library(
    FawkesLib::Console ALIAS FawkesLibConsole
    )

# Create the __FILENAME__ macro for use with the logger
define_file_basename_for_sources(${PROJECT_NAME})

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        FawkesLib::Core
        readline
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
