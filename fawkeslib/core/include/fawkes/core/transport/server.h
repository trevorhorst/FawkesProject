#ifndef FAWKES_SERVER_H
#define FAWKES_SERVER_H

#include "fawkes/core/control/control_template.h"

namespace Fawkes
{

class ServerTemplate
        : public ControlTemplate< ServerTemplate >
{
public:
    virtual ~ServerTemplate();
    virtual int32_t listen() = 0;
};

}

#endif // FAWKES_SERVER_H
