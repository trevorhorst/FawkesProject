#ifndef FAWKES_COMMAND_H
#define FAWKES_COMMAND_H

#include <string.h>

#include "fawkes/core/control/control.h"

#define COMMAND_NAME_MAX    1024

namespace Fawkes
{

class Command
{
public:
    Command( const char *name );

    const char *name();

    int32_t applyName( const char *name );

private:
    char mName[ COMMAND_NAME_MAX ];
};

}

#endif // FAWKES_COMMAND_H
