cmake_minimum_required( VERSION 3.5 )

# Need to figure out how to get this properly implemented
# find_package( FawkesLibCore REQUIRED )

project( FawkesLibExample
    VERSION 0.0.1
    LANGUAGES C CXX
    DESCRIPTION "Fawkes Library example application"
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

enable_language( C ASM )

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( FAWKES_EXAMPLE_INCLUDE_DIR include/example )
set( FAWKES_EXAMPLE_SOURCE_DIR src )

set( FAWKES_EXAMPLE_INCLUDES ${FAWKES_EXAMPLE_RESOURCES}
        ${FAWKES_EXAMPLE_INCLUDE_DIR}/resources/resources.h
    )

set( FAWKES_EXAMPLE_SOURCES ${FAWKES_EXAMPLE_RESOURCES}
        ${FAWKES_EXAMPLE_SOURCE_DIR}/main.cpp
        ${FAWKES_EXAMPLE_SOURCE_DIR}/resources/index.html.s
        ${FAWKES_EXAMPLE_SOURCE_DIR}/resources/bundle.js.s
    )

add_executable( 
    ${PROJECT_NAME}
        ${FAWKES_EXAMPLE_INCLUDES}
        ${FAWKES_EXAMPLE_SOURCES}
    )

define_file_basename_for_sources(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/resources>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        FawkesLib::Core
        FawkesLib::Console
        FawkesLib::Http
        pthread
    )
