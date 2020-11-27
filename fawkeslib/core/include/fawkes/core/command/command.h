#ifndef FAWKES_COMMAND_H
#define FAWKES_COMMAND_H

#include <string.h>
#include <functional>

#include "fawkes/core/control/control.h"

#define COMMAND_NAME_MAX    1024

namespace Fawkes
{

class Command
{
public:

    using ParameterCallback = std::function< int32_t (cJSON*) >;
    using ParameterMap      = Types::CharHashMap< ParameterCallback >;

    Command( const char *name );
    virtual ~Command();

    const char *name();

    int32_t applyName( const char *name );

    virtual int32_t call( cJSON *params, cJSON *response ) = 0;

protected:
    const ParameterMap *requiredMap();
    const ParameterMap *mutableMap();

    ParameterMap mMutableMap;
    ParameterMap mRequiredMap;

private:
    char mName[ COMMAND_NAME_MAX ];

};

}

#endif // FAWKES_COMMAND_H
