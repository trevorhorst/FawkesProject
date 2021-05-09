cmake_minimum_required( VERSION 3.5 )

# Need to figure out how to get this properly implemented
# find_package( FawkesLibCore REQUIRED )

project( FawkesLibExample
    VERSION 0.0.1
    LANGUAGES C CXX
    DESCRIPTION "Fawkes Library example application"
    )

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
