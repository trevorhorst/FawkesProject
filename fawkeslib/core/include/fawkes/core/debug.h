#ifndef FAWKES_DEBUG_H
#define FAWKES_DEBUG_H

#include "fawkes/core/control/control_template.h"
#include "fawkes/core/logger/log.h"

namespace Fawkes
{

class Debug
        : public ControlTemplate< Debug >
{
public:
    Debug();

    int32_t setLevel( int32_t level );

    int32_t level();
};

}

#endif // FAWKES_DEBUG_H
