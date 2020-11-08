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
            LOG_ERROR("read_history failed (%d) - (%s)", errno, strerror(errno) );
            error = Error::Type::CTRL_OPERATION_FAILED;
        }
    }

    // Set the readline callback handler
    // rl_callback_handler_install( ">", &Console::evaluate );

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

    if( !error ) {
        if( write_history( hfile ) ) {
            LOG_ERROR("write_history failed (%d) - (%s)", errno, strerror(errno) );
            error = Error::Type::CTRL_OPERATION_FAILED;
        } else {
            // Keep the history to minimum.
            history_truncate_file( hfile, HISTORY_LINES_MAX );
        }
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

}
