#include "fawkes/core/control/control_template.h"

namespace Fawkes
{

class Client
        : public ControlTemplate< Client >
{
public:
    virtual ~Client();
    virtual int32_t send( const char *data ) = 0;
};

}
