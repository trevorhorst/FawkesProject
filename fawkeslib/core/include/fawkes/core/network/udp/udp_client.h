#ifndef FAWKES_UDP_CLIENT_H
#define FAWKES_UDP_CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <functional>
#include <unistd.h>

#include "fawkes/core/transport/client.h"

namespace Fawkes
{

class UdpClient
        : public Client
{
public:
    UdpClient();

    int32_t open( int16_t port, const char *address = nullptr );
    int32_t send( const char *data, char *response, int32_t responseLength ) override;

    int32_t applyPort( int16_t port );

private:
    int16_t mPort;
    int32_t mSocket;
};

}

#endif // FAWKES_UDP_CLIENT_H
