#include "fawkes/core/network/unix/unix.h"

namespace Fawkes
{

const char *UnixServer::socket_name = "socket";

UnixServer::UnixServer()
    : mDone( false )
{

}

int32_t UnixServer::run()
{
    int32_t error = Error::NONE;

    int32_t sock = 0;
    int32_t messageSocket = 0;

    // Describes the local unix socket
    struct sockaddr_un server;

    sock = socket( AF_UNIX, SOCK_STREAM, 0 );
    if( sock < 0 ) {
        LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
        error = Error::Type::CTRL_OPERATION_FAILED;
    }

    if( !error ) {
        server.sun_family = AF_UNIX;
        strcpy( server.sun_path, socket_name );
        if( bind( sock, (struct sockaddr *)&server, sizeof( struct sockaddr_un ) ) ) {
            // Socket stream failed to bind
            LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
            error = Error::Type::CTRL_OPERATION_FAILED;
        }
    }

    if( !error ) {
        LOG_INFO( "Socket has name %s", server.sun_path );

        // Begin listening. Server will queue 5 messages before it stops listening
        listen( sock, 5 );
        while( !mDone ) {
            char buffer[ 1024 ];
            int32_t message = accept( sock, 0, 0 );
            if( message < 0 ) {
                LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
                error = Error::Type::CTRL_OPERATION_FAILED;
            } else {
                bzero( buffer, sizeof( buffer ) );
                int32_t messager = read(message, buffer, 1024);
                if( messager < 0) {
                    // Failed to read stream message
                    LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
                    error = Error::Type::CTRL_OPERATION_FAILED;
                } else if( messager == 0 ) {
                    printf("Ending connection\n");
                } else {
                    printf("-->%s\n", buffer);
                }
            } // while (rval > 0);
            close( message );
        }
    }

    close( sock );

    return error;
}

}
