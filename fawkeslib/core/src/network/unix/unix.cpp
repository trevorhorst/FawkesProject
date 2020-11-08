#include "fawkes/core/network/unix/unix.h"

namespace Fawkes
{

UnixServer::UnixServer()
{

}

int32_t UnixServer::listen()
{
    int32_t error = Error::NONE;

    int32_t sock = 0;
    int32_t messageSocket = 0;

    sock = socket( AF_UNIX, SOCK_STREAM, 0 );

    close( sock );

    return error;
}

}
