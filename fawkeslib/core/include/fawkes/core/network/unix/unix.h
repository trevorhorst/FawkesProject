#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "fawkes/core/transport/server.h"

namespace Fawkes
{

class UnixServer
        : Server
{
    static const char *socket_name;

public:
    UnixServer();
    int32_t run() override;

private:
    bool mDone;

};

}
