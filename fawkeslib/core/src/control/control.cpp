#include "fawkes/core/control/control.h"

namespace Fawkes
{

const uint32_t Control::log_trace = 0x1;
const uint32_t Control::log_debug = 0x2;
const uint32_t Control::log_info  = 0x4;

/**
 * @brief Constructor
 */
Control::Control()
    : mLog( 0 )
{

}

/**
 * @brief Destructor
 */
Control::~Control()
{

}

/**
 * @brief Applies a log flag setting
 * @param flag Desired log flags to set. May set multiple in a single call.
 */
void Control::setLogFlags( uint32_t flag )
{
    mLog |= flag;
}

/**
 * @brief Reports true if trace messages are enabled
 * @return bool
 */
bool Control::trace()
{
    return ( mLog & log_trace );
}

/**
 * @brief Reports true if debug messages are enabled
 * @return
 */
bool Control::debug()
{
    return ( mLog & log_debug );
}

/**
 * @brief Reports true if info messages are enabled
 * @return
 */
bool Control::info()
{
    return ( mLog & log_info );
}

}
