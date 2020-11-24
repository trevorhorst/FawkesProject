#include "fawkes/core/network/unix/unix_server.h"

namespace Fawkes
{

const char *UnixServer::default_socket_name = "/tmp/unix_socket";
const char *UnixServer::success_response    = "success";

/**
 * @brief UnixServer::UnixServer Constructor
 * @param socketPath Desired server socket path
 */
UnixServer::UnixServer( const char *socketPath )
    : mDone( false )
    , mListening( false )
    , mSocketPath{ 0 }
    , mSocket( -1 )
{
    applySocketPath( socketPath );
}

/**
 * @brief UnixServer::~UnixServer Destructor
 */
UnixServer::~UnixServer()
{
    stop();
}

/**
 * @brief UnixServer::stop Stops the server from listening
 */
void UnixServer::stop()
{
    if( listening() ) {
        mListening = false;
    }
}

/**
 * @brief UnixServer::listening Retrieves the listening status of the server
 * @return bool
 */
bool UnixServer::listening()
{
    return mListening;
}

/**
 * @brief UnixServer::socketPath Retrieves the socket path
 * @return const char pointer
 */
const char *UnixServer::socketPath()
{
    return mSocketPath;
}

/**
 * @brief UnixServer::listen Start listening on the socket
 * @return Error code
 */
int32_t UnixServer::listen()
{
    int32_t error = Error::Type::NONE;

    struct sockaddr_un server;
    char buf[ 1024 ] = { 0 };

    // Open a new datagram unix socket
    int32_t sock = socket( AF_UNIX, SOCK_DGRAM, 0 );
    if( sock < 0 ) {
        LOG_ERROR( "%s (%d)", strerror( errno ), errno );
        return Error::Type::CTRL_OPERATION_FAILED;
    }

    server.sun_family = AF_UNIX;
    strcpy( server.sun_path, socketPath() );

    if( bind( sock, (struct sockaddr *)&server, sizeof( struct sockaddr_un ) ) ) {
        // Socket stream failed to bind
        LOG_ERROR( "Socket failed to bind: %s (%d)", strerror( errno ), errno );
        return Error::Type::CTRL_OPERATION_FAILED;
    } else {
        LOG_INFO( "Server socket bound to %s", server.sun_path );
    }

    // Prepare a socket for client response
    struct sockaddr_un clientAddr;
    socklen_t clientLen = sizeof( clientAddr );

    // Server is now listening
    mListening = true;

    fd_set fds;
    while( listening() ) {

        // Configure the timeout for select
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 50000;

        // Initialize the fd set
        FD_ZERO( &fds );
        FD_SET( sock, &fds );

        // Start select on the socket
        if( select( FD_SETSIZE , &fds, nullptr, nullptr, &tv ) < 0 ) {
            LOG_ERROR( "select failed: %s (%d)", strerror( errno ), errno );
            error = Error::Type::CTRL_OPERATION_FAILED;
            // Step listening, the loop will exit on next iteration
            stop();
        }

        if( FD_ISSET( sock, &fds )) {
            //File descriptor is ready, read characters
            if( recvfrom( sock, buf, sizeof( buf ), 0, (struct sockaddr*)&clientAddr, &clientLen ) < 0 ) {
                LOG_ERROR( "Datagram read failed: %s (%d)", strerror( errno ), errno );
            } else {
                LOG_INFO( "Datagram received: %s", buf );
                LOG_INFO( "Datagram info: %s", clientAddr.sun_path );
                if( sendto( sock
                        , success_response, strlen( success_response )
                        , 0
                        , reinterpret_cast< struct sockaddr* >( &clientAddr )
                        , sizeof( struct sockaddr_un ) ) < 0 ) {
                    LOG_ERROR( "%s (%d)", strerror( errno ), errno );
                } else {
                    LOG_INFO( "Response sent" );
                }
            }
        }
    }

    LOG_INFO( "Cleaning up socket: %s", server.sun_path );

    close( sock );
    unlink( server.sun_path );

    return error;
}

/// @note Will come back to a SOCK_STREAM setup
// int32_t UnixServer::stream()
// {
//     int32_t error = Error::NONE;
//
//     int32_t sock = 0;
//
//     // Describes the local unix socket
//     struct sockaddr_un server;
//
//     sock = socket( AF_UNIX, SOCK_STREAM, 0 );
//     if( sock < 0 ) {
//         LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
//         error = Error::Type::CTRL_OPERATION_FAILED;
//     }
//
//     if( !error ) {
//         server.sun_family = AF_UNIX;
//         strcpy( server.sun_path, socketPath() );
//         if( bind( sock, (struct sockaddr *)&server, sizeof( struct sockaddr_un ) ) ) {
//             // Socket stream failed to bind
//             LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
//             error = Error::Type::CTRL_OPERATION_FAILED;
//         }
//     }
//
//     if( !error ) {
//         LOG_INFO( "Socket has name %s", server.sun_path );
//
//         // Begin listening. Server will queue 5 messages before it stops listening
//         listen( sock, 5 );
//         while( !mDone ) {
//             char buffer[ 1024 ];
//             int32_t message = accept( sock, 0, 0 );
//             if( message < 0 ) {
//                 LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
//                 error = Error::Type::CTRL_OPERATION_FAILED;
//             } else {
//                 bzero( buffer, sizeof( buffer ) );
//                 int32_t messager = read(message, buffer, 1024);
//                 if( messager < 0) {
//                     // Failed to read stream message
//                     LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
//                     error = Error::Type::CTRL_OPERATION_FAILED;
//                 } else if( messager == 0 ) {
//                     printf("Ending connection\n");
//                 } else {
//                     printf("-->%s\n", buffer);
//                     mDone = true;
//                 }
//             } // while (rval > 0);
//             close( message );
//         }
//     }
//
//     close( sock );
//
//     return error;
// }

/**
 * @brief UnixServer::applySocketPath Applies the socket path
 * @param path Desired socket path
 * @return Error code
 */
int32_t UnixServer::applySocketPath( const char *path )
{
    int32_t error = Error::Type::NONE;

    if( path != nullptr ) {
        strncpy( mSocketPath, path, PATH_MAX - 1 );
        mSocketPath[ PATH_MAX - 1 ] = '\0';
    } else {
        LOG_WARN( "Desired socket path is NULL" );
        error = Error::Type::CTRL_OPERATION_FAILED;
    }

    return error;
}

}
