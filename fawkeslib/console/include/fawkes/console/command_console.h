#ifndef FAWKES_COMMAND_CONSOLE_H
#define FAWKES_COMMAND_CONSOLE_H

#include "fawkes/core/command/command_template.h"
#include "fawkes/console/console.h"

#define COMMAND_NAME_CONSOLE    "console"
#define COMMAND_NAME_QUIT       "quit"

namespace Fawkes
{

class CommandConsole
        : public CommandTemplate< Console >
{
public:
    CommandConsole();

    int32_t setQuit( cJSON *json );

};

}

#endif // FAWKES_COMMAND_CONSOLE_H
