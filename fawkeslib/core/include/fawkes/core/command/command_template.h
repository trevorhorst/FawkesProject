#ifndef FAWKES_COMMAND_TEMPLATE_H
#define FAWKES_COMMAND_TEMPLATE_H

#include "fawkes/core/command/command.h"

namespace Fawkes {

template< typename T >
class CommandTemplate
        : public Command
{
public:

    /**
     * @brief CommandTemplate Constructor
     * @param name Name of the command
     */
    CommandTemplate( const char *name )
        : Command( name )
        , mControlObject( nullptr )
    {
        if( T::count() == 1 ) {
            // Initialize the control object to the first item in the list
            mControlObject = static_cast< T* >( T::at( 0 ) );
        }
    }

    /**
     * @brief call Handle a call to the command
     * @param params cJSON formatted parameter list
     * @param response cJSOn formatted response container
     * @return Error code
     */
    int32_t call( cJSON *params, cJSON *response ) override
    {
        int32_t error = Error::Type::NONE;

        // Iterate over the required parameters
        error = processParameterMap( params, requiredMap() );

        if( error == Error::Type::NONE ) {
            if( mControlObject ) {
                processParameterMap( params, optionalMap() );
            } else {
                LOG_WARN( "Controle object is not available" );
                error = Error::Type::CMD_FAILED;
            }
        }

        /*
        if( error && ( params->child == nullptr ) ) {
            /// @todo If the parameter has no child, treat it as an accessor
            LOG_TRACE( "PARAMS IS NULL" );
        } else {
            /// @todo If the parameter has a child, treat it as a mutator
            LOG_TRACE( "PARAMS IS NOT NULL" );
        }
        */

        return error;
    }

protected:
    T *mControlObject;

};

}

#endif // FAWKES_COMMAND_TEMPLATE_H
