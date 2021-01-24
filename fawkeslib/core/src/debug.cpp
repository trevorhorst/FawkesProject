#include "fawkes/core/debug.h"

namespace Fawkes
{

Debug::Debug()
    : ControlTemplate< Debug >()
{

}

int32_t Debug::setLevel( int32_t level )
{
    int32_t error = Error::Type::NONE;
    if( ( level >= LOG_TRACE ) && ( level <= LOG_FATAL ) ) {
        log_set_level( level );
    } else {
        error = Error::Type::PARAM_OUT_OF_RANGE;
    }
    return error;
}

int32_t Debug::level()
{
    return log_level();
}

}
