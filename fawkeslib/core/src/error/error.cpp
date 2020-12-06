#include "fawkes/core/error/error.h"

namespace Fawkes {

const char* Error::type_text[] = {
    "No Error"
    , "Syntax Error"
    , "Command Invalid"
    , "Command Missing"
    , "Command Failed"
    , "Parameter Invalid"
    , "Parameter Missing"
    , "Parameter Out of Range"
    , "Parameter Wrong Type"
    , "Parameter Access Denied"
    , "Control Missing"
    , "Generic Error"
};

const char *Error::toString(int type)
{
    return toString( static_cast< Type >( type ) );
}

const char *Error::toString( Type type )
{
    return type_text[ type ];
}

}
