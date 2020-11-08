#ifndef FAWKES_CONSOLE_H
#define FAWKES_CONSOLE_H

#define HISTORY_LINES_MAX 1000

#include <sys/types.h>
#include <termio.h>
#include <unistd.h>
#include <string>

#include <readline/readline.h>
#include <readline/history.h>

#include "fawkes/core/control/control_template.h"

#define CONSOLE_DELIMITER_DEFAULT " "

namespace Fawkes
{

class Console : public ControlTemplate< Console >
{
    static const char *history_file;
    static const char *history_file_location;

public:
    Console();

    int32_t run();
    int32_t quit();
    static void evaluate( char *input );
    static std::vector< std::string > tokenize(
            char *input, const char *delimiter = CONSOLE_DELIMITER_DEFAULT );

private:
    bool mDone;

};

}

#endif // FAWKES_CONSOLE_H
