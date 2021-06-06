#ifndef FAWKES_UNIX_CLIENT_H
#define FAWKES_UNIX_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>

#include "fawkes/core/transport/client.h"

namespace Fawkes
{

class UnixClient
        : public ClientTemplate
{
    static const char *default_socket_path;
    static const char *hello_world;

public:
    UnixClient();

    int32_t send( const char *data, char *response, int32_t responseLength ) override;
    int32_t stream( const char *data );

    const char *socketPath();
    const char *socketDestinationPath();

    int32_t applySocketPath( const char *path );
    int32_t applySocketDestinationPath( const char *path );

private:
    char mSocketPath[ PATH_MAX ];
    char mSocketDestinationPath[ PATH_MAX ];

};

}

#endif // FAWKES_UNIX_CLIENT_H
