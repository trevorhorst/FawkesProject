#ifndef FAWKES_CLIENT_H
#define FAWKES_CLIENT_H

#include "fawkes/core/control/control_template.h"

namespace Fawkes
{

class Client
        : public ControlTemplate< Client >
{
public:
    virtual ~Client();
    virtual int32_t send( const char *data ) = 0;
};

}

#endif // FAWKES_CLIENT_H
