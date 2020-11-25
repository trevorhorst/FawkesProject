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
#include <thread>

#include "fawkes/core/option_parser.h"
#include "fawkes/core/json/cjson.h"
#include "fawkes/core/logger/log.h"
#include "fawkes/console/console.h"
#include "fawkes/core/network/unix/unix_client.h"
#include "fawkes/core/network/unix/unix_server.h"

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

int32_t serverCallback( const char *data )
{
    cJSON *parsed = cJSON_Parse( data );
    if( parsed == nullptr ) {
        LOG_WARN( "Command string is invalid: %s", data );
    } else {
        char *json = cJSON_Print( parsed );
        LOG_INFO( "\n%s", json );
        cJSON_free( json );
    }
    cJSON_Delete( parsed );
    return 0;
}

void testConsole()
{
    cJSON *test = cJSON_CreateObject();
    cJSON_AddStringToObject( test, "hello", "world" );

    char *hello = cJSON_PrintUnformatted( test );
    LOG_INFO( "%s", hello );

    free( hello );
    cJSON_Delete( test );

    Fawkes::UnixServer server;
    server.applyCommandCallback( std::bind( serverCallback, std::placeholders::_1 ) );
    Fawkes::UnixClient client;
    client.applySocketDestinationPath( server.socketPath() );

    Fawkes::Console *console = &Fawkes::Console::getInstance();
    console->applyClient( &client );

    std::thread *serverThread = new std::thread( &Fawkes::UnixServer::listen, &server );
    std::thread *app = new std::thread( &Fawkes::Console::run, console );

    app->join();
    server.stop();
    serverThread->join();

    delete app;
    delete serverThread;
}

void testUnixServer()
{
    Fawkes::UnixServer server;
    Fawkes::UnixClient client;

    std::thread *serverThread = new std::thread( &Fawkes::UnixServer::listen, &server );
    usleep(1000000);

    client.send( "hello" );


    serverThread->join();
}

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

    // testUnixServer();
    testConsole();

    return 0;
}
