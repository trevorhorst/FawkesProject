#include "fawkes/core/control/control_template.h"

namespace Fawkes
{

class Server
        : public ControlTemplate< Server >
{
public:
    virtual ~Server();
    virtual int32_t listen() = 0;
};

}
