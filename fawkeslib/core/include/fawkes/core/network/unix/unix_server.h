#ifndef FAWKES_UNIX_SERVER
#define FAWKES_UNIX_SERVER

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <functional>

#include "fawkes/core/transport/server.h"

namespace Fawkes
{

class UnixServer
        : Server
{
    static const char *default_socket_name;
    static const char *success_response;

public:
    using CommandCallback = std::function< int32_t ( const char * ) >;

    UnixServer( const char *socketName = default_socket_name );
    ~UnixServer();

    void stop();
    int32_t listen() override;
    int32_t stream();

    bool listening();
    const char *socketPath();

    int32_t applySocketPath( const char *path );
    int32_t applyCommandCallback( CommandCallback callback );

private:
    bool mDone;
    bool mListening;

    char mSocketPath[ PATH_MAX ];
    int32_t mSocket;
    CommandCallback mCommandCallback;

    int32_t defaultHandler( const char *data );

};

}

#endif // FAWKES_UNIX_SERVER
