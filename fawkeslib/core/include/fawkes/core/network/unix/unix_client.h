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
        : Client
{
public:
    UnixClient();

    int32_t send(const char *data) override;

private:
    static const char *hello_world;

};

}

#endif // FAWKES_UNIX_CLIENT_H
