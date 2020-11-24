#include "fawkes/core/network/unix/unix_server.h"

namespace Fawkes
{

const char *UnixServer::default_socket_name = "socket";
const char *UnixServer::success_response    = "success";

UnixServer::UnixServer( const char *socketName )
    : mDone( false )
    , mListening( false )
    , mSocketName( socketName )
{

}

UnixServer::~UnixServer()
{
    stop();
}

void UnixServer::stop()
{
    if( listening() ) {
        mListening = false;
    }
}

bool UnixServer::listening()
{
    return mListening;
}

const char *UnixServer::socketName()
{
    return mSocketName;
}

int32_t UnixServer::run()
{
    int32_t error = Error::Type::NONE;

    struct sockaddr_un server;
    char buf[ 1024 ] = {0};

    int32_t sock = socket( AF_UNIX, SOCK_DGRAM, 0 );
    if( sock < 0 ) {
        LOG_ERROR( "%s (%d)", strerror( errno ), errno );
        return Error::Type::CTRL_OPERATION_FAILED;
    }

    server.sun_family = AF_UNIX;
    strcpy( server.sun_path, socketName() );

    if( bind( sock, (struct sockaddr *)&server, sizeof( struct sockaddr_un ) ) ) {
        // Socket stream failed to bind
        LOG_ERROR( "Socket failed to bind: %s (%d)", strerror( errno ), errno );
        return Error::Type::CTRL_OPERATION_FAILED;
    } else {
        LOG_INFO( "Server socket bound to %s", server.sun_path );
    }

    // Server is now listening
    mListening = true;

    struct sockaddr_un clientAddr;
    socklen_t clientLen = sizeof( clientAddr );

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

    LOG_INFO( "Cleaning up socket: %s", server.sun_path );

    close( sock );
    unlink( server.sun_path );

    return error;
}

int32_t UnixServer::stream()
{
    int32_t error = Error::NONE;

    int32_t sock = 0;

    // Describes the local unix socket
    struct sockaddr_un server;

    sock = socket( AF_UNIX, SOCK_STREAM, 0 );
    if( sock < 0 ) {
        LOG_ERROR( "Socket failure (%d) - (%s)", errno, strerror( errno ) );
        error = Error::Type::CTRL_OPERATION_FAILED;
    }

    if( !error ) {
        server.sun_family = AF_UNIX;
        strcpy( server.sun_path, socketName() );
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
                    mDone = true;
                }
            } // while (rval > 0);
            close( message );
        }
    }

    close( sock );

    return error;
}

}
