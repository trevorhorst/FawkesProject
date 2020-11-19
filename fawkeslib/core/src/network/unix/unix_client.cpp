#include "fawkes/core/network/unix/unix_client.h"

namespace Fawkes
{

const char *UnixClient::hello_world = "helloworld";

UnixClient::UnixClient()
{

}

int32_t UnixClient::send(const char *data = hello_world)
{
    int32_t error = Error::NONE;

    int sock;
    struct sockaddr_un server;
    // char buf[1024];

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
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

}
