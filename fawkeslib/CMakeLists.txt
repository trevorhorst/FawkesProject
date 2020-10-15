cmake_minimum_required( VERSION 3.5 )
project( FawkesLib
    VERSION 0.0.1
    LANGUAGES CXX
    DESCRIPTION "Fawkes Library"
    )

# Creates a truncated filename macro for use with the logger
# https://stackoverflow.com/questions/8487986/file-macro-shows-full-path
set( CMAKE_CXX_FLAGS
    "-D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'"
    )

add_subdirectory( core )

add_subdirectory( example )
