#include "fawkes/console/command_help.h"

namespace Fawkes
{

CommandHelp::CommandHelp()
    : CommandTemplate<CommandHandler>( COMMAND_NAME_HELP, Type::QUERY )
{
    mAccessableMap[ COMMAND_NAME_MAP ] = BIND_PARAMETER( &CommandHelp::map );
}

int32_t CommandHelp::map( cJSON *json )
{
    int32_t error = Error::Type::NONE;

    const Types::CharMap< Command * > map = mControlObject->sortedMap();

    cJSON *array = cJSON_CreateArray();
    // cJSON_AddItemToObject( json, COMMAND_NAME_MAP, array );

    for( auto it = map.begin(); it != map.end(); it++ ) {
        cJSON *str = cJSON_CreateString( it->first );
        cJSON_AddItemToArray( array, str );
    }

    cJSON_AddItemToObject( json, "", array );

    return error;
}

}
