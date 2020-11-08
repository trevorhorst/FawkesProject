#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "fawkes/core/transport/server.h"

namespace Fawkes
{

class UnixServer
        : Server
{
public:
    UnixServer();
    int32_t listen() override;

};

}
