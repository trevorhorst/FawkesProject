#ifndef FAWKES_COMMAND_HELP_H
#define FAWKES_COMMAND_HELP_H

#include "fawkes/core/command/command_template.h"
#include "fawkes/core/command/command_handler.h"

#define COMMAND_NAME_HELP   "help"
#define COMMAND_NAME_MAP    "map"

namespace Fawkes
{

class CommandHelp
        : public CommandTemplate< CommandHandler >
{
public:
    CommandHelp();

    int32_t map( cJSON *json );

};

}

#endif // FAWKES_COMMAND_HELP_H
