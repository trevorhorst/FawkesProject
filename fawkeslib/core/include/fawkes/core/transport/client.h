#ifndef FAWKES_CLIENT_H
#define FAWKES_CLIENT_H

#include "fawkes/core/control/control_template.h"

namespace Fawkes
{

class ClientTemplate
        : public ControlTemplate< ClientTemplate >
{
public:
    virtual ~ClientTemplate();
    virtual int32_t send( const char *data, char *response, int32_t responseLength ) = 0;
};

}

#endif // FAWKES_CLIENT_H
