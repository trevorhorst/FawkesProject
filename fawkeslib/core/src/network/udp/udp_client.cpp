#include "fawkes/core/network/udp/udp_client.h"

namespace Fawkes
{

UdpClient::UdpClient()
    : Client()
    , mPort( 0 )
{

}

int32_t UdpClient::send( const char *data, char *response, int32_t responseLength )
{
    int32_t error = 0;


    int32_t sock = socket( AF_INET, SOCK_DGRAM, 0 );
    if( sock < 0 ) {
        error = -1;
        LOG_ERROR( "%s (%d)", strerror( errno ), errno );
    } else {
        LOG_TRACE( "Client socket opened" );
    }

    if( error >= 0 ) {
        // Configure
        struct sockaddr_in serverAddress;
        memset( &serverAddress, 0, sizeof( serverAddress ) );
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr   = INADDR_ANY;
        serverAddress.sin_port = htons( mPort );

        if( sendto( sock, data, strlen( data )
                , 0, (const struct sockaddr *) &serverAddress,
                sizeof( serverAddress ) ) < 0 ) {
            LOG_ERROR( "Failed to send datagram: %s (%d)", strerror( errno ), errno );
        } else {
            LOG_DEBUG( "Datagram sent... waiting for response" );
            if( recvfrom( sock, response, responseLength, 0, nullptr, nullptr ) < 0 ) {
                LOG_ERROR( "%s (%d)", strerror( errno ), errno );
            } else {
                LOG_INFO( "Client: %s", data );
            }
        }
    }

    // Clean up our client socket
    close( sock );

    return error;
}

int32_t UdpClient::applyPort( int16_t port )
{
    int32_t error = 0;
    mPort = port;
    return error;
}

}
