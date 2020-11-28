#include "fawkes/console/command_console.h"

namespace Fawkes
{

CommandConsole::CommandConsole()
    : CommandTemplate< Console >( COMMAND_NAME_CONSOLE )
{
    mOptionalMap[ COMMAND_NAME_QUIT ] = std::bind( &CommandConsole::setQuit, this, std::placeholders::_1 );
}

int32_t CommandConsole::setQuit( cJSON *json )
{
    int32_t error = Error::Type::NONE;
    if( cJSON_IsBool( json ) ) {
        error = mControlObject->quit();
    }
    return error;
}

}
