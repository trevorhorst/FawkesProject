#include "fawkes/core/command/command.h"

namespace Fawkes
{

/**
 * @brief Command::Command Constructor
 * @param name Desired name of the command
 * @param type Type of command
 */
Command::Command( const char *name, Type type )
    : mName{ 0 }
    , mType( type )
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

Command::Type Command::type()
{
    return mType;
}

const Command::ParameterMap *Command::requiredMap()
{
    return &mRequiredMap;
}

const Command::ParameterMap *Command::optionalMap()
{
    return &mOptionalMap;
}

/**
 * @brief Command::processParameterMap Processes the information found in a
 *  parameter map
 * @param parameters Incoming parameters to process
 * @param response Results of processing the data
 * @param map
 * @return
 */
int32_t Command::processParameterMap( cJSON *parameters, cJSON *response
                                      , cJSON *details, const ParameterMap *map )
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
            // Parameter NOT found
            if( map == requiredMap() ) {
                // If the parameter map is required, report as an error
                LOG_DEBUG( "Required parameter missing: %s", parameterName );
                error = Error::Type::PARAM_MISSING;
            }
        } else {
            // Parameter found, perform callback to handle data and
            // propogate error
            error = parameterCallback( parameterData );

        }

        if( error ) {
            cJSON_AddStringToObject( details, "type", Error::toString( error ) );
            cJSON_AddStringToObject( details, "details", parameterName );
            cJSON_AddItemToObject( response, "error", details );
        }

        cJSON_Delete( parameterData );
    }

    return error;
}

int32_t Command::queryParameterMap( cJSON *parameters, cJSON *response
                                    , cJSON *details, const ParameterMap *map )
{
    int32_t error = Error::Type::NONE;

    for( auto it = map->begin()
         ; ( it != map->end() ) && ( error == Error::Type::NONE )
         ; it++ ) {

        const char *parameterName = it->first;
        ParameterCallback parameterCallback = it->second;

        error = parameterCallback( response );

        // cJSON_AddItemToObject( response, parameterName, parameterData );
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
