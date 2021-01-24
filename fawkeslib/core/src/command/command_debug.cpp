#include "fawkes/core/command/command_debug.h"

namespace Fawkes {

CommandDebug::CommandDebug()
    : CommandTemplate< Debug >( COMMAND_NAME_DEBUG, Type::CONTROL )
{
    mMutableMap[ COMMAND_NAME_LEVEL ] = BIND_PARAMETER( &CommandDebug::setLevel );

    mAccessableMap[ COMMAND_NAME_LEVEL ] = BIND_PARAMETER( &CommandDebug::level );
}

int32_t CommandDebug::setLevel( cJSON *json )
{
    int32_t error = Error::Type::NONE;
    if( cJSON_IsNumber( json ) ) {
        error = mControlObject->setLevel( json->valuedouble );
    } else {
        error = Error::Type::PARAM_INVALID;
    }
    return error;
}

int32_t CommandDebug::level(cJSON *json)
{
    int32_t error = Error::Type::NONE;
    cJSON_AddNumberToObject( json, COMMAND_NAME_LEVEL, mControlObject->level() );
    return error;
}

}
