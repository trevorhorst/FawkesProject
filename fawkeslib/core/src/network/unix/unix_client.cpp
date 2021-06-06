#include "fawkes/core/network/unix/unix_client.h"

namespace Fawkes
{

const char *UnixClient::default_socket_path = "/tmp/unix_client";
const char *UnixClient::hello_world = "helloworld";

/**
 * @brief UnixClient::UnixClient Constructor
 */
UnixClient::UnixClient()
    : ClientTemplate()
    , mSocketPath{ 0 }
    , mSocketDestinationPath{ 0 }
{
    applySocketPath( default_socket_path );
}

/**
 * @brief UnixClient::send Send a string of data
 * @param data Null terminated string of data to send
 * @return Error status
 */
int32_t UnixClient::send( const char *data, char *response , int32_t responseLength )
{
    int32_t error = Error::Type::NONE;

    // Create unix datagram socket
    struct sockaddr_un clientAddr;
    int32_t sock = socket( AF_UNIX, SOCK_DGRAM, 0 );
    if( sock < 0 ) {
        LOG_ERROR( "%s (%d)", strerror( errno ), errno );
        return Error::Type::CTRL_OPERATION_FAILED;
    }

    // Configure client socket info
    clientAddr.sun_family = AF_UNIX;
    strcpy( clientAddr.sun_path, socketPath() );

    // Create temporary client socket to bind to for communication
    if( bind( sock, (struct sockaddr *)&clientAddr, sizeof( struct sockaddr_un ) ) ) {
        // Socket stream failed to bind
        LOG_ERROR( "Socket failed to bind: %s (%d)", strerror( errno ), errno );
        return Error::Type::CTRL_OPERATION_FAILED;
    } else {
        LOG_DEBUG( "Client socket bound to %s", clientAddr.sun_path );
    }

    // Configure server socket info
    struct sockaddr_un serverAddr;
    serverAddr.sun_family = AF_UNIX;
    strcpy( serverAddr.sun_path, socketDestinationPath() );

    if( sendto( sock, data, strlen( data ), 0,
        (struct sockaddr*)&serverAddr, sizeof(struct sockaddr_un)) < 0) {
        // perror("sending datagram message");
        LOG_ERROR( "Failed to send datagram: %s (%d)", strerror( errno ), errno );
    } else {
        LOG_DEBUG( "Datagram sent... waiting for response" );
        if( recvfrom( sock, response, responseLength, 0, nullptr, nullptr ) < 0 ) {
            LOG_ERROR( "%s (%d)", strerror( errno ), errno );
        } else {
            // LOG_INFO( "Client: %s", buf );
        }
    }

    // Clean up our client socket
    close( sock );
    unlink( clientAddr.sun_path );

    return error;
}

/// @note Will come back to a SOCK_STREAM setup
// int32_t UnixClient::stream(const char *data = hello_world)
// {
//     int32_t error = Error::NONE;
//
//     int sock;
//     struct sockaddr_un server;
//     // char buf[1024];
//
//     sock = socket(AF_UNIX, SOCK_STREAM, 0);
//     if( sock < 0 ) {
//         perror("opening stream socket");
//         LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
//         error = Error::Type::CTRL_OPERATION_FAILED;
//     } else {
//         server.sun_family = AF_UNIX;
//         strcpy(server.sun_path, "socket");
//     }
//
//     if( !error ) {
//         if( connect( sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0) {
//             close(sock);
//             perror("connecting stream socket");
//             LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
//             error = Error::Type::CTRL_OPERATION_FAILED;
//         }
//         if( write( sock, data, sizeof( data ) ) < 0 ) {
//             perror("writing on stream socket");
//             LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
//         }
//     }
//
//     close(sock);
//
//     return error;
// }

/**
 * @brief UnixClient::socketPath Retrieves the socket path
 * @return const char pointer
 */
const char *UnixClient::socketPath()
{
    return mSocketPath;
}

/**
 * @brief UnixClient::socketDestinationPath Retrieves the socket destination path
 * @return const char pointer
 */
const char *UnixClient::socketDestinationPath()
{
    return mSocketDestinationPath;
}

/**
 * @brief UnixClient::applySocketPath Applies the socket path
 * @param path Desired socket path
 * @return Error code
 */
int32_t UnixClient::applySocketPath( const char *path )
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

/**
 * @brief UnixClient::applySocketDestinationPath Applies the destination path
 * @param path Desired destination path
 * @return Error code
 */
int32_t UnixClient::applySocketDestinationPath( const char *path )
{
    int32_t error = Error::Type::NONE;

    if( path != nullptr ) {
        strncpy( mSocketDestinationPath, path, PATH_MAX - 1 );
        mSocketDestinationPath[ PATH_MAX - 1 ] = '\0';
    } else {
        LOG_WARN( "Desired socket path is NULL" );
        error = Error::Type::CTRL_OPERATION_FAILED;
    }

    return error;
}

}
