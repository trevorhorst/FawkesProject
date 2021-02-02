#include "fawkes/core/network/udp/udp_server.h"

namespace Fawkes
{

UdpServer::UdpServer()
    : Server()
    , mListening( false )
    , mPort( 0 )
    , mCommandCallback( nullptr )
{

}

UdpServer::~UdpServer()
{

}

void UdpServer::stop()
{
    mListening = false;
}

bool UdpServer::listening()
{
    return mListening;
}

int32_t UdpServer::listen()
{
    int32_t error = 0;

    listen( mPort );

    return 0;
}

int32_t UdpServer::listen( int32_t port )
{
    int32_t error = 0;

    char buffer[ UDP_SERVER_BUFFER_SIZE ] = {};

    // Open a socket
    int32_t sock = socket( AF_INET, SOCK_DGRAM, 0 );
    if( sock < 0 ) {
        error = -1;
        LOG_ERROR( "%s (%d)", strerror( errno ), errno );
        return error;
    }

    // Initialize the server information
    struct sockaddr_in servaddr;
    memset( &servaddr, 0, sizeof( servaddr ) );
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr   = INADDR_ANY;
    servaddr.sin_port = htons( port );

    // Bind the socket with the server address
    if( bind( sock, reinterpret_cast< const struct sockaddr *>( &servaddr ),
                   sizeof( servaddr ) ) < 0 ) {
        error = -1;
        LOG_ERROR( "%s (%d)", strerror( errno ), errno );
        return error;
    } else {
        LOG_INFO( "Server socket bound to %d", port );
    }

    mListening = true;

    struct sockaddr_in clientAddress;
    socklen_t clientLength = sizeof( clientAddress );

    fd_set fds;
    while( error >= 0
           && listening() ) {

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

            // File descriptor is ready, read characters
            if( recvfrom( sock, buffer, sizeof( buffer ), 0
                          , (struct sockaddr*)&clientAddress, &clientLength ) < 0 ) {
                LOG_ERROR( "Datagram read failed: %s (%d)", strerror( errno ), errno );
            } else {

                char *response = nullptr;

                if( responseHandler() ) {
                    responseHandler()( buffer, &response );
                }

                if( response == nullptr ) {
                    LOG_WARN( "Command response is NULL" );
                } else if( sendto( sock
                        , response, strlen( response )
                        , 0
                        , reinterpret_cast< struct sockaddr* >( &clientAddress )
                        , sizeof( struct sockaddr ) ) < 0 ) {
                    LOG_ERROR( "%s (%d)", strerror( errno ), errno );
                } else {
                    LOG_TRACE( "Response sent" );
                }

                if( response ) {
                    // Clean up any response that may have been created at the
                    // lower levels
                    free( response );
                }
            }
        }
    }

    return error;
}

UdpServer::CommandCallback UdpServer::responseHandler()
{
    return mCommandCallback;
}

int32_t UdpServer::applyPort(uint16_t port)
{
    int32_t error = Error::Type::NONE;
    mPort = port;
    return error;
}

int32_t UdpServer::applyResponseHandler( CommandCallback callback )
{
    int32_t error = Error::Type::NONE;
    mCommandCallback = callback;
    return error;
}

}
