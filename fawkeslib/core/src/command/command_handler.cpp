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
int32_t CommandHandler::process( const char *data )
{
    int32_t error = Error::Type::NONE;

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
        // Check if the command exists in our map
        auto it = map()->find( cmd->valuestring );
        if( it == map()->end() ) {
            // The command does not exist
            LOG_WARN( "Command is invalid: %s", cmd->valuestring );
            error = Error::Type::CMD_INVALID;
        } else {
            cJSON *response = cJSON_CreateObject();

            // Check the json data for a parameter entry
            cJSON *params = cJSON_GetObjectItem( parsedData, COMMAND_NAME_PARAMS );
            it->second->call( params, response );
            cJSON_Delete( response );
        }
    }

    // Clean up our parsed json data
    cJSON_Delete( parsedData );

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
