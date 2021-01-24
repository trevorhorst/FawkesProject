#ifndef FAWKES_COMMAND_H
#define FAWKES_COMMAND_H

#include <string.h>
#include <functional>

#include "fawkes/core/control/control.h"

#define COMMAND_NAME_MAX    1024

#define BIND_PARAMETER( X ) std::bind( X, this, std::placeholders::_1 )

namespace Fawkes
{

class Command
{
public:

    enum Type
    {
        CONTROL = 0,
        QUERY   = 1
    };

    using ParameterCallback = std::function< int32_t (cJSON*) >;
    using ParameterMap      = Types::CharHashMap< ParameterCallback >;

    Command( const char *name, Type type );
    virtual ~Command();

    const char *name();
    Type type();

    int32_t applyName( const char *name );

    virtual int32_t call( cJSON *params, cJSON *response, cJSON *details ) = 0;
    virtual int32_t setup();
    virtual int32_t teardown();
    virtual int32_t processParameterMap( cJSON *parameters, cJSON *response
                                         , cJSON *details, const ParameterMap *map );
    virtual int32_t mutate( cJSON *parameters, cJSON *response, cJSON *details );
    virtual int32_t access( cJSON *response, cJSON *details );

protected:
    const ParameterMap *requiredMap();
    const ParameterMap *optionalMap();
    const ParameterMap *mutableMap();
    const ParameterMap *accessableMap();

    ParameterMap mRequiredMap;
    ParameterMap mOptionalMap;
    ParameterMap mMutableMap;
    ParameterMap mAccessableMap;

private:
    char mName[ COMMAND_NAME_MAX ];
    Type mType;

};

}

#endif // FAWKES_COMMAND_H
