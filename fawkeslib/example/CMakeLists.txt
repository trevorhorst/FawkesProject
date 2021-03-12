cmake_minimum_required( VERSION 3.5 )

# Need to figure out how to get this properly implemented
# find_package( FawkesLibCore REQUIRED )

project( FawkesLibExample
    VERSION 0.0.1
    LANGUAGES C CXX
    DESCRIPTION "Fawkes Library example application"
    )

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_executable( 
    ${PROJECT_NAME}
        main.cpp
    )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        FawkesLib::Core
        FawkesLib::Console
        FawkesLib::Http
        pthread
    )
