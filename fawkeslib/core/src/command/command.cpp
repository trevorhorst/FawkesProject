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

Command::~Command()
{

}

const char *Command::name()
{
    return mName;
}

const Command::ParameterMap *Command::requiredMap()
{
    return &mRequiredMap;
}

const Command::ParameterMap *Command::optionalMap()
{
    return &mOptionalMap;
}

int32_t Command::processParameterMap( cJSON *parameters, const ParameterMap *map )
{
    int32_t error = Error::Type::NONE;

    // Iterate over the required parameters
    for( auto it = map->begin()
         ; ( it != map->end() ) && ( error == Error::Type::NONE )
         ; it++ ) {

        // Retrieve parameter information
        const char *parameterName = it->first;
        ParameterCallback parameterCallback = it->second;

        // Check if the parameter exists within the list of params received
        cJSON *parameterData = cJSON_DetachItemFromObject( parameters, parameterName );
        if( parameterData == nullptr ) {
            // Parameter NOT found, report it and set error
            LOG_DEBUG( "Required parameter missing: %s", parameterName );
            error = Error::Type::PARAM_MISSING;
        } else {
            // Parameter found, perform callback to handle data and
            // propogate error
            error = parameterCallback( parameterData );
        }

        cJSON_Delete( parameterData );
    }

    return error;
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
