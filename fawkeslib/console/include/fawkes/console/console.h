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
#include "fawkes/core/singleton.h"
#include "fawkes/core/transport/client.h"

#define CONSOLE_DELIMITER_DEFAULT " "

namespace Fawkes
{

class Console
        : public ControlTemplate< Console >
        , public Singleton< Console >
{
    friend class Singleton< Console >;

    static const char *history_file;
    static const char *history_file_location;

public:

    int32_t run();
    int32_t quit();

    int32_t applyClient( Client *client );

    static void evaluate( char *input );
    static std::vector< std::string > tokenize(
            char *input, const char *delimiter = CONSOLE_DELIMITER_DEFAULT );

    Client *client();

private:
    Console();

    bool mDone;
    Client *mClient;

};

}

#endif // FAWKES_CONSOLE_H
