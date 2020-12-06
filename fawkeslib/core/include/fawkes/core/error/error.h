#ifndef FAWKES_ERROR_H
#define FAWKES_ERROR_H

namespace Fawkes
{

class Error
{
    static const char *type_text[];

public:

    enum Type {
        NONE    = 0
        , SYNTAX
        , CMD_INVALID
        , CMD_MISSING
        , CMD_FAILED
        , PARAM_INVALID
        , PARAM_MISSING
        , PARAM_OUT_OF_RANGE
        , PARAM_WRONG_TYPE
        , PARAM_ACCESS_DENIED
        , CTRL_OPERATION_FAILED
        , GENERIC
        , MAX
    };

    static const char *toString( int type );
    static const char *toString( Type type );

};

}

#endif // FAWKES_ERROR_H
