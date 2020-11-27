#ifndef FAWKES_COMMAND_HANDLER_H
#define FAWKES_COMMAND_HANDLER_H

#include <vector>

#include "fawkes/core/json/cjson.h"
#include "fawkes/core/command/command.h"
#include "fawkes/core/common_types.h"

#define COMMAND_NAME_CMD    "cmd"

namespace Fawkes
{

class CommandHandler
{
public:
    CommandHandler();

    int32_t process( const char *data );

    Types::CharHashMap< Command* > *map();

private:
    Types::CharHashMap< Command* > mCommandMap;
};

}

#endif // FAWKES_COMMAND_HANDLER_H
