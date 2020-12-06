#include "fawkes/core/command/command_handler.h"

namespace Fawkes
{

/**
 * @brief CommandHandler::CommandHandler Constructor
 */
CommandHandler::CommandHandler()
{

}

/**
 * @brief CommandHandler::map Retrieves a pointer to the command map
 * @return
 */
const Types::CharHashMap< Command* > *CommandHandler::map()
{
    return &mCommandMap;
}

/**
 * @brief CommandHandler::process Processes incoming command data
 * @param data Incoming data
 * @return Error code
 */
int32_t CommandHandler::process( const char *data , char **response )
{
    int32_t error = Error::Type::NONE;

    // Container to capture responseJson data
    cJSON *responseJson = cJSON_CreateObject();

    // Parse the input data
    cJSON *parsedData = cJSON_Parse( data );
    cJSON *cmd = nullptr;

    // Check validity of the json data
    if( parsedData == nullptr ) {
        LOG_WARN( "Unable to process input: %s", data );
        error = Error::Type::CTRL_OPERATION_FAILED;
    } else {
        // Check the json data for a command entry
        cmd = cJSON_GetObjectItem( parsedData, COMMAND_NAME_CMD );
    }

    if( error || !cJSON_IsString( cmd ) ) {
        LOG_WARN( "Malformed JSON pattern" );
        error = Error::Type::CTRL_OPERATION_FAILED;
    } else {
        // Add the command name to the response string
        cJSON_AddStringToObject( responseJson, COMMAND_NAME_CMD, cmd->valuestring );

        cJSON *details = cJSON_CreateObject();
        cJSON *results = cJSON_CreateObject();

        // Check if the command exists in our map
        auto it = map()->find( cmd->valuestring );
        if( it == map()->end() ) {
            // The command does not exist
            LOG_WARN( "Command is invalid: %s", cmd->valuestring );
            error = Error::Type::CMD_INVALID;
            cJSON_AddStringToObject( details, "type", Error::toString( error ) );
            cJSON_AddStringToObject( details, "details", cmd->valuestring );
        } else {
            // Check the json data for a parameter entry
            cJSON *params = cJSON_GetObjectItem( parsedData, COMMAND_NAME_PARAMS );
            error = it->second->call( params, results, details );
        }

        if( error ) {
            // Add the error object to the response json
            cJSON_AddItemToObject( responseJson, "error", details );
            // Delete the unused results
            cJSON_Delete( results );
        } else {
            // Add the results object to the response json
            cJSON_AddItemToObject( responseJson, "results", results );
            // Delete the unused error details
            cJSON_Delete( details );
        }

    }


    cJSON_AddBoolToObject( responseJson, COMMAND_NAME_SUCCESS, error ? false : true );

    // This will need to be cleaned up at the top level
    *response = cJSON_PrintUnformatted( responseJson );

    // Clean up our parsed json data
    cJSON_Delete( parsedData );
    cJSON_Delete( responseJson );

    return error;
}

/**
 * @brief CommandHandler::addCommand Add a new command
 * @param command Pointer to desired command
 */
void CommandHandler::addCommand( Command *command )
{
    if( command ) {
        // Add the command, if it exists
        mCommandMap[ command->name() ] = command;
    }
}

}
