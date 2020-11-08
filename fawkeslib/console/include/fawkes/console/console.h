#ifndef FAWKES_CONSOLE_H
#define FAWKES_CONSOLE_H

#define HISTORY_LINES_MAX 1000

#include <sys/types.h>
#include <termio.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "fawkes/core/control/control_template.h"

namespace Fawkes
{

class Console : public ControlTemplate< Console >
{
    static const char *history_file;
    static const char *history_file_location;

public:
    Console();

    int32_t run();

private:
    bool mDone;

};

}

#endif // FAWKES_CONSOLE_H
