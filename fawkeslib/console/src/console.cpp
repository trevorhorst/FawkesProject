#include "fawkes/console/console.h"

namespace Fawkes
{

const char *Console::history_file = "fawkes_console.log";
const char *Console::history_file_location = "/tmp";

/**
 * @brief Constructor
 */
Console::Console()
    : mDone( false )
    , mClient( nullptr )
{

}

int32_t Console::run()
{
    int32_t error = 0;

    struct termios term;

    tcflag_t old_lflag;
    cc_t old_vtime;

    char hfile[ FILENAME_MAX ];

    // Retrieve the current terminal attributes for standard input
    if( tcgetattr( STDIN_FILENO, &term ) < 0 ) {
        LOG_ERROR("tcgetattr failed (%d) - (%s)", errno, strerror(errno) );
        error = Error::Type::CTRL_OPERATION_FAILED;
    }

    if( !error ) {
        // Save off current settings
        old_lflag = term.c_lflag;
        old_vtime = term.c_cc[ VTIME ];

        // AND out the ICANON flag - Place the terminal for standard input
        // in non canonical mode
        term.c_lflag &= ~ICANON;
        term.c_cc[ VTIME ] = 1;
        if( tcsetattr( STDIN_FILENO, TCSANOW, &term ) < 0 ) {
            LOG_ERROR("tcsetattr failed (%d) - (%s)", errno, strerror(errno) );
            error = Error::Type::CTRL_OPERATION_FAILED;
        }
    }

    if( !error ) {
        snprintf( hfile, sizeof( hfile ), "%s/%s"
                  , history_file_location, history_file );
        if( read_history( hfile ) ) {
            LOG_INFO("read_history failed (%d) - (%s)", errno, strerror(errno) );
            error = Error::Type::CTRL_OPERATION_FAILED;
        }
    }

    // Set the readline callback handler
    rl_callback_handler_install( ">", &Console::evaluate );

    fd_set fds;
    while( !mDone ) {
        // Set the timeout
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 50000;
        // Zeroize file descriptors
        FD_ZERO( &fds );
        // Set the file descriptor to the standard input
        FD_SET( fileno( stdin ), &fds );

        /// @note select() and pselect() allow a program to monitor multiple file
        /// descriptors, waiting until one or more of the file descriptors become
        /// "ready" for some class of I/O operation (e.g., input possible).
        if( select( FD_SETSIZE, &fds, nullptr, nullptr, &tv ) < 0 ) {
            LOG_ERROR("select failed (%d) - (%s)", errno, strerror(errno) );
            error = Error::Type::CTRL_OPERATION_FAILED;
            break;
        }

        if( FD_ISSET( fileno( stdin ), &fds ) ) {
            // File descriptor is ready, read characters
            rl_callback_read_char();
        } else {
            // If we don't find any new data
        }
    }

    if( write_history( hfile ) ) {
        LOG_ERROR("write_history failed (%d) - (%s)", errno, strerror(errno) );
        error = Error::Type::CTRL_OPERATION_FAILED;
    } else {
        // Keep the history to minimum.
        history_truncate_file( hfile, HISTORY_LINES_MAX );
    }

    // Restore previous flag settings
    term.c_lflag     = old_lflag;
    term.c_cc[VTIME] = old_vtime;
    // Set the terminal attributes
    if( tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0 ) {
        LOG_ERROR("tcsetattr failed (%d) - (%s)", errno, strerror(errno) );
        error = Error::Type::CTRL_OPERATION_FAILED;
    }

    rl_callback_handler_remove();

    LOG_INFO("Exitting...");
    return error;
}

int32_t Console::quit()
{
    int32_t error = Error::NONE;
    mDone = true;
    /// @note Uncommenting this prevents the additional prompt print, revist
    /// this later down the road to cleanup output
    rl_callback_handler_remove();
    return error;
}

bool Console::done()
{
    return mDone;
}

int32_t Console::applyClient( Client *client )
{
    int32_t error = Error::Type::NONE;
    mClient = client;
    return error;
}

void Console::evaluate( char *input )
{
    Console *console = &Console::getInstance();

    // Tokenize the input
    std::vector< std::string > tokenized = tokenize( input );

    if( tokenized.empty() ) {
        // The input is empty
    } else if( tokenized.at( 0 ) == "quit" ) {
        // The command is to quit, so lets quit. Nothing fancy here.
        console->quit();
    } else {

        // Add the cmd string to the object
        cJSON *msg    = cJSON_CreateObject();
        cJSON *params = cJSON_CreateObject();
        cJSON *cmd    = nullptr;

        for( auto it = tokenized.begin(); it != tokenized.end(); it++ ) {
            if( *it == tokenized.at( 0 ) ) {
                // Add the command parameter
                cmd = cJSON_CreateString( (*it).c_str() );
            } else {
                auto t = it;
                it++;
                if( it == tokenized.end() ) {
                    printf( "Parameter mismatch\n" );
                    break;
                } else {
                    // Parse the parameter
                    cJSON *param = cJSON_Parse( (*it).c_str() );
                    // Add the item to the parameter list
                    cJSON_AddItemToObject( params, (*t).c_str(), param );
                }
            }
        }

        cJSON_AddItemToObject( msg, "cmd", cmd );
        cJSON_AddItemToObject( msg, "params", params );

        char *msgStr = cJSON_PrintUnformatted( msg );

        if( console->client() ) {
            console->client()->send( msgStr );
        } else {
            LOG_INFO( "No client available" );
        }

        cJSON_free( msgStr );

        // Clean up the message, this should delete all the components
        cJSON_Delete( msg );

    }

    if( input ) {
        // Actual input exists
        if( input[ 0 ] != '\0' ) {
            // The input is more than just an empty string
            add_history( input );
        }
        free( input );
    }
}

std::vector< std::string > Console::tokenize( char *input, const char *delimiter )
{
    std::string str( input );
    std::string token;
    std::vector< std::string > tokenized;
    size_t last = 0;
    size_t next = 0;

    // Split the input into segments by the delimeter
    while( ( next = str.find( delimiter, last ) ) != std::string::npos ) {
        if( next - last != 0 ) {
            // There is data in the string
            token = str.substr( last, next - last );
            tokenized.push_back( token );
        } else {
            // If the next and last values are the same, the string is empty
            // and we can ignore it
        }
        last = next + 1;
    }


    if( !str.substr( last ).empty() ) {
        // Need to place the the final substring in the list, unless it's empty
        tokenized.push_back( str.substr( last ) );
    }

    return tokenized;
}

Client *Console::client()
{
    return mClient;
}

}
