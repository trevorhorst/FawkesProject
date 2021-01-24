#ifndef FAWKES_COMMAND_DEBUG_H
#define FAWKES_COMMAND_DEBUG_H

#include "fawkes/core/command/command_template.h"
#include "fawkes/core/debug.h"

#define COMMAND_NAME_DEBUG  "debug"
#define COMMAND_NAME_QDEBUG "qdebug"
#define COMMAND_NAME_LEVEL  "level"

namespace Fawkes
{

class CommandDebug
        : public CommandTemplate< Debug >
{
public:
    CommandDebug();

    int32_t setLevel( cJSON *json );

    int32_t level( cJSON *json );
};

}

#endif
