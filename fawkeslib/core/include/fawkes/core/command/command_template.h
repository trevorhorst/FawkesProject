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
    CommandTemplate( const char *name, Type type )
        : Command( name, type )
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
    int32_t call( cJSON *params, cJSON *response, cJSON *details ) override
    {
        int32_t error = Error::Type::NONE;

        setup();

        // Iterate over the required parameters
        error = processParameterMap( params, response, details, requiredMap() );

        if( error == Error::Type::NONE )  {
            if( mControlObject ) {
                // There is a control object to operate with
                if( params->child == nullptr ) {
                    // There are no more parameters, query the control
                    error = access( response, details );
                } else {
                    // There are more parameters, mutate the control
                    error = mutate( params, response, details );
                }

            } else {
                LOG_WARN( "Control object is not available" );
                error = Error::Type::CMD_FAILED;
                cJSON_AddNumberToObject( details, "code", error );
                cJSON_AddStringToObject( details, "type", Error::toString( error ) );
                cJSON_AddStringToObject( details, "details", "Control is unavailable" );
            }
        }

        teardown();

        return error;
    }

protected:
    T *mControlObject;

};

}

#endif // FAWKES_COMMAND_TEMPLATE_H
