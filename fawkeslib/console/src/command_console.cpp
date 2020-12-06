#include "fawkes/console/command_console.h"

namespace Fawkes
{

CommandConsole::CommandConsole()
    : CommandTemplate< Console >( COMMAND_NAME_CONSOLE, Type::CONTROL )
{
    mOptionalMap[ COMMAND_NAME_QUIT ] = BIND_PARAMETER( &CommandConsole::quit );
}

int32_t CommandConsole::quit( cJSON *json )
{
    int32_t error = Error::Type::NONE;
    if( cJSON_IsBool( json ) ) {
        if( cJSON_IsTrue( json ) ) {
            error = mControlObject->quit();
        }
    } else {
        error = Error::Type::PARAM_WRONG_TYPE;
    }
    return error;
}

CommandQConsole::CommandQConsole()
    : CommandTemplate<Console>( COMMAND_NAME_QCONSOLE, Type::QUERY )
{
    mOptionalMap[ COMMAND_NAME_DONE] = BIND_PARAMETER( &CommandQConsole::done );
}

int32_t CommandQConsole::done( cJSON *json )
{
    int32_t error = Error::Type::NONE;
    cJSON_AddBoolToObject( json, COMMAND_NAME_DONE, mControlObject->done() );
    return error;
}

}
