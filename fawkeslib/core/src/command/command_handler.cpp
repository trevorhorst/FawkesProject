#include "fawkes/core/command/command_handler.h"

namespace Fawkes
{

CommandHandler::CommandHandler()
{

}

Types::CharHashMap< Command* > *CommandHandler::map()
{
    return &mCommandMap;
}

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
        // Check the json data for a command parameter
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

        }
    }

    cJSON_Delete( parsedData );
    return error;
}

}
