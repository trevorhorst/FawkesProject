#ifndef FAWKES_COMMAND_TEST_H
#define FAWKES_COMMAND_TEST_H

#include "fawkes/core/command/command_template.h"
#include "fawkes/core/network/unix/unix_server.h"

#define COMMAND_NAME_TEST   "test"

namespace Fawkes
{

class CommandTest
        : public CommandTemplate< UnixServer >
{
public:
    CommandTest();
    virtual int32_t setParam( cJSON *data );
};

}

#endif // FAWKES_COMMAND_TEST_H
