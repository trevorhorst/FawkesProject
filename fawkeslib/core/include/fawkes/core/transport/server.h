#ifndef FAWKES_SERVER_H
#define FAWKES_SERVER_H

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

#endif // FAWKES_SERVER_H
