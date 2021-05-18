cmake_minimum_required( VERSION 3.5 )
project(
    FawkesLibHttp
    VERSION 0.0.1
    LANGUAGES CXX C
    DESCRIPTION "Fawkes Project HTTP library"
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
set( FAWKES_HTTP_INCLUDE_DIR include/fawkes/http )
set( FAWKES_HTTP_SOURCE_DIR src )

set( FAWKES_HTTP_INCLUDE ${FAWKES_HTTP_INCLUDE}
    ${FAWKES_HTTP_INCLUDE_DIR}/client.h
    ${FAWKES_HTTP_INCLUDE_DIR}/http.h
    ${FAWKES_HTTP_INCLUDE_DIR}/request.h
    ${FAWKES_HTTP_INCLUDE_DIR}/response.h
    ${FAWKES_HTTP_INCLUDE_DIR}/route.h
    ${FAWKES_HTTP_INCLUDE_DIR}/router.h
    ${FAWKES_HTTP_INCLUDE_DIR}/server.h
    )

set( FAWKES_HTTP_SOURCE ${FAWKES_HTTP_SOURCE}
    ${FAWKES_HTTP_SOURCE_DIR}/client.cpp
    ${FAWKES_HTTP_SOURCE_DIR}/http.cpp
    ${FAWKES_HTTP_SOURCE_DIR}/request.cpp
    ${FAWKES_HTTP_SOURCE_DIR}/response.cpp
    ${FAWKES_HTTP_SOURCE_DIR}/route.cpp
    ${FAWKES_HTTP_SOURCE_DIR}/router.cpp
    ${FAWKES_HTTP_SOURCE_DIR}/server.cpp
    )

add_library(
    ${PROJECT_NAME}
    SHARED
        ${FAWKES_HTTP_INCLUDE}
        ${FAWKES_HTTP_SOURCE}
    )

# Create the __FILENAME__ macro for use with the logger
define_file_basename_for_sources(${PROJECT_NAME})

# Alias out the library to include the namespace
add_library(
    FawkesLib::Http ALIAS FawkesLibHttp
    )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        FawkesLib::Core
        microhttpd
        curl
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

