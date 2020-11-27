#include "fawkes/core/command/command.h"

namespace Fawkes
{

/**
 * @brief Command::Command Constructor
 * @param name Desired name of the command
 */
Command::Command( const char *name )
    : mName{ 0 }
{
    applyName( name );
}

int32_t Command::applyName( const char *name )
{
    int32_t error = Error::Type::NONE;

    if( name != nullptr ) {
        strncpy( mName, name, COMMAND_NAME_MAX - 1 );
        mName[ COMMAND_NAME_MAX - 1 ] = '\0';
    } else {
        LOG_WARN( "Command name is invalid" );
        error = Error::Type::CTRL_OPERATION_FAILED;
    }

    return error;
}

}
