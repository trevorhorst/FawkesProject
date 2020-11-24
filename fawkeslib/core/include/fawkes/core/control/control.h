#ifndef FAWKES_CONTROL_H
#define FAWKES_CONTROL_H

#include <stdint.h>
#include <errno.h>
#include <linux/limits.h>

#include "fawkes/core/json/cjson.h"
#include "fawkes/core/logger/log.h"
#include "fawkes/core/error/error.h"

namespace Fawkes
{

class Control
{
public:

    static const uint32_t log_trace;
    static const uint32_t log_debug;
    static const uint32_t log_info;

    Control();

    // Polymorphic classes should have virtual destructors. This help ensures
    // proper cleanup of objects
    virtual ~Control();

    void setLogFlags( uint32_t flag );

    bool trace();
    bool debug();
    bool info();

private:
    uint32_t mLog;

};

}

#endif // FAWKES_CONTROL_H
