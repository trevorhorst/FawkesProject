#include "fawkes/core/command/command_test.h"

namespace Fawkes
{

CommandTest::CommandTest()
    : CommandTemplate<UnixServer>( COMMAND_NAME_TEST, Type::CONTROL )
{
    mRequiredMap[ "param" ] = std::bind( &CommandTest::setParam, this, std::placeholders::_1 );
}

int32_t CommandTest::setParam( cJSON *data )
{
    int32_t error = Error::Type::NONE;

    char *printData = cJSON_Print( data );
    LOG_TRACE( "%s : %s", __FUNCTION__, printData );
    free( printData );

    return error;
}

}
