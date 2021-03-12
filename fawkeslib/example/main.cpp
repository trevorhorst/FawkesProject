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
#include "fawkes/core/network/unix/unix_client.h"
#include "fawkes/core/network/unix/unix_server.h"
#include "fawkes/core/command/command_handler.h"
#include "fawkes/core/command/command_test.h"
#include "fawkes/core/command/command_debug.h"

#include "fawkes/console/console.h"
#include "fawkes/console/command_console.h"
#include "fawkes/console/command_help.h"

#include "fawkes/http/server.h"
#include "fawkes/http/client.h"

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

void testHttp()
{
    Fawkes::Debug debug;
    debug.setLevel( LOG_INFO );

    Fawkes::CommandHandler handler;

    Fawkes::HttpServer server;
    server.applyCommandCallback( std::bind( &Fawkes::CommandHandler::process
                                            , &handler
                                            , std::placeholders::_1
                                            , std::placeholders::_2 ) );

    Fawkes::HttpClient client;

    // Create a console instance and provide a client for use by the server
    Fawkes::Console *console = &Fawkes::Console::getInstance();
    console->applyClient( &client );

    Fawkes::CommandConsole cmdConsole;
    Fawkes::CommandHelp cmdHelp;
    Fawkes::CommandTest cmdTest;
    Fawkes::CommandDebug cmdDebug;
    handler.addCommand( &cmdTest );
    handler.addCommand( &cmdConsole );
    handler.addCommand( &cmdHelp );
    handler.addCommand( &cmdDebug );

    server.listen();
    std::thread *appThread = new std::thread( &Fawkes::Console::run, console );

    appThread->join();

    delete appThread;
}

void testConsole()
{
    Fawkes::Debug debug;
    // Allows only INFO statements or higher to be printed
    debug.setLevel( LOG_INFO );

    Fawkes::CommandHandler handler;

    // Create a server and provide a command handler callback
    Fawkes::UnixServer server;
    // server.applyCommandCallback( std::bind( serverCallback, std::placeholders::_1 ) );
    server.applyCommandCallback( std::bind( &Fawkes::CommandHandler::process
                                            , &handler
                                            , std::placeholders::_1
                                            , std::placeholders::_2 ) );

    // Create a unix socket client and provide a path for socket creation
    Fawkes::UnixClient client;
    client.applySocketDestinationPath( server.socketPath() );

    // Create a console instance and provide a client for use by the server
    Fawkes::Console *console = &Fawkes::Console::getInstance();
    console->applyClient( &client );

    // Make sure the commands are initialized after their control objects
    Fawkes::CommandConsole cmdConsole;
    Fawkes::CommandHelp cmdHelp;
    Fawkes::CommandTest cmdTest;
    Fawkes::CommandDebug cmdDebug;
    handler.addCommand( &cmdTest );
    handler.addCommand( &cmdConsole );
    handler.addCommand( &cmdHelp );
    handler.addCommand( &cmdDebug );

    std::thread *serverThread = new std::thread( &Fawkes::UnixServer::listen, &server );
    std::thread *appThread = new std::thread( &Fawkes::Console::run, console );

    appThread->join();

    // Once the application thread has stopped, stop the server from listening
    server.stop();
    serverThread->join();

    delete appThread;
    delete serverThread;
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
    // testConsole();
    testHttp();

    return 0;
}
