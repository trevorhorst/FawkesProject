/** ****************************************************************************
 * @file main.cpp
 * @author Trevor Horst
 * @copyright
 * @brief Entry point for the Fawkes Library example application
 * ****************************************************************************/
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "fawkes/core/option_parser.h"
#include "fawkes/core/json/cjson.h"
#include "fawkes/core/logger/log.h"

enum OptionIndex {
    UNKNOWN  = 0
    , HELP
    , NUM_ARGUMENTS
};

const Option::Descriptor usage[] = {
    { UNKNOWN, 0, "" , "", Option::Arg::None, "USAGE: FawkesLibExample [options]\n\n"
                                               "Options:" },
    { HELP, 0, "h" , "help", Option::Arg::None, "  --help  \tPrint usage and exit." },
    { 0, 0, nullptr, nullptr, nullptr, nullptr }
};


int main( int argc, char *argv[] )
{
    argc -= ( argc > 0 );
    argv += ( argc > 0 );

    Option::Stats stats( usage, argc, argv );
    std::vector< Option::Option > options( stats.options_max );
    std::vector< Option::Option > buffer( stats.buffer_max );
    Option::Parser parse( usage, argc, argv, &options[ 0 ], &buffer[ 0 ] );

    if( parse.error() ) {
        // Print error and return early with error
        LOG_ERROR( "Unable to parse optional arguments" );
        return 1;
    }

    if( options[ HELP ] ) {
        // Print usage and return early
        Option::printUsage( std::cout, usage );
        return 0;
    }

    cJSON *test = cJSON_CreateObject();
    cJSON_AddStringToObject( test, "hello", "world" );

    char *hello = cJSON_PrintUnformatted( test );
    LOG_INFO( "%s", hello );

    free( hello );
    cJSON_Delete( test );

    return 0;
}
