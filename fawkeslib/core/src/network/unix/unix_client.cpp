#include "fawkes/core/network/unix/unix_client.h"

namespace Fawkes
{

const char *UnixClient::default_socket_path = "/tmp/unix_client";
const char *UnixClient::hello_world = "helloworld";

UnixClient::UnixClient()
{

}

int32_t UnixClient::send( const char *data )
{
    int32_t error = Error::Type::NONE;

    struct sockaddr_un serverAddr;

    // Create unix datagram socket
    struct sockaddr_un clientAddr;
    int32_t sock = socket( AF_UNIX, SOCK_DGRAM, 0 );
    if( sock < 0 ) {
        LOG_ERROR( "%s (%d)", strerror( errno ), errno );
        return Error::Type::CTRL_OPERATION_FAILED;
    }

    // Configure client socket info
    clientAddr.sun_family = AF_UNIX;
    strcpy( clientAddr.sun_path, default_socket_path );

    // Create temporary client socket for communication
    if( bind( sock, (struct sockaddr *)&clientAddr, sizeof( struct sockaddr_un ) ) ) {
        // Socket stream failed to bind
        LOG_ERROR( "Socket failed to bind: %s (%d)", strerror( errno ), errno );
        return Error::Type::CTRL_OPERATION_FAILED;
    } else {
        LOG_INFO( "Client socket bound to %s", clientAddr.sun_path );
    }

    // Configure server socket info
    serverAddr.sun_family = AF_UNIX;
    strcpy( serverAddr.sun_path, "socket" );

    if( sendto( sock, data, strlen( data ), 0,
        (struct sockaddr*)&serverAddr, sizeof(struct sockaddr_un)) < 0) {
        // perror("sending datagram message");
        LOG_ERROR( "Failed to send datagram: %s (%d)", strerror( errno ), errno );
    } else {
        LOG_INFO( "Datagram sent... waiting for response" );
        char buf[1024];
        if( recvfrom( sock, buf, sizeof( buf ), 0, nullptr, nullptr ) < 0 ) {
            LOG_ERROR( "%s (%d)", strerror( errno ), errno );
        } else {
            LOG_INFO( "Client: %s", buf );
        }
    }

    close( sock );
    unlink( clientAddr.sun_path );

    return error;
}

int32_t UnixClient::stream(const char *data = hello_world)
{
    int32_t error = Error::NONE;

    int sock;
    struct sockaddr_un server;
    // char buf[1024];

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if( sock < 0 ) {
        perror("opening stream socket");
        LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
        error = Error::Type::CTRL_OPERATION_FAILED;
    } else {
        server.sun_family = AF_UNIX;
        strcpy(server.sun_path, "socket");
    }

    if( !error ) {
        if( connect( sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0) {
            close(sock);
            perror("connecting stream socket");
            LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
            error = Error::Type::CTRL_OPERATION_FAILED;
        }
        if( write( sock, data, sizeof( data ) ) < 0 ) {
            perror("writing on stream socket");
            LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
        }
    }

    close(sock);

    return error;
}

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

}
