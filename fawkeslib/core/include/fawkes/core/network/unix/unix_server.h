#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#include "fawkes/core/transport/server.h"

namespace Fawkes
{

class UnixServer
        : Server
{
    static const char *default_socket_name;
    static const char *success_response;

public:
    UnixServer( const char *socketName = default_socket_name );
    ~UnixServer();

    void stop();
    int32_t listen() override;
    int32_t stream();

    bool listening();
    const char *socketPath();

    int32_t applySocketPath( const char *path );

private:
    bool mDone;
    bool mListening;

    char mSocketPath[ PATH_MAX ];
    int32_t mSocket;

};

}
