#ifndef FAWKES_COMMAND_HANDLER_H
#define FAWKES_COMMAND_HANDLER_H

#include <vector>

#include "fawkes/core/json/cjson.h"
#include "fawkes/core/command/command.h"
#include "fawkes/core/common_types.h"
#include "fawkes/core/control/control_template.h"

#define COMMAND_NAME_CMD        "cmd"
#define COMMAND_NAME_PARAMS     "params"
#define COMMAND_NAME_RESULTS    "results"
#define COMMAND_NAME_SUCCESS    "success"

namespace Fawkes
{

class CommandHandler
        : public ControlTemplate< CommandHandler >
{
public:
    CommandHandler();

    void addCommand( Command *command );
    int32_t process( const char *data, char **response );

    const Types::CharHashMap< Command* > *map();
    const Types::CharMap< Command* > sortedMap();

private:
    Types::CharHashMap< Command* > mCommandMap;
};

}

#endif // FAWKES_COMMAND_HANDLER_H
