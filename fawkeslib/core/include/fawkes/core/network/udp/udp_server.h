#ifndef FAWKES_UDP_SERVER_H
#define FAWKES_UDP_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <functional>

#include "fawkes/core/transport/server.h"

#define UDP_SERVER_BUFFER_SIZE  4096

namespace Fawkes
{

class UdpServer
        : public Server
{
public:
    using CommandCallback = std::function< int32_t ( const char *, char ** ) >;

    UdpServer();
    ~UdpServer();

    void stop();
    bool listening();
    int32_t listen() override;
    int32_t listen( int32_t port );
    int32_t receive();
    uint32_t port();

    CommandCallback responseHandler();

    int32_t applyPort( uint16_t port );
    int32_t applyResponseHandler( CommandCallback callback );

private:
    bool mListening;

    uint16_t mPort;
    CommandCallback mCommandCallback;

    int32_t defaultResponseHandler( const char *data, char **response );
};

}

#endif // FAWKES_UDP_SERVER_H
