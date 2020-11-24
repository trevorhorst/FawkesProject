#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

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
    bool listening();
    const char *socketName();
    int32_t run() override;
    int32_t stream();

private:
    bool mDone;
    bool mListening;
    const char *mSocketName;

};

}
