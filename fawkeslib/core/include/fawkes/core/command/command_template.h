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
        for( auto it = requiredMap()->begin()
             ; ( it != requiredMap()->end() ) && ( error == Error::Type::NONE )
             ; it++ ) {

            // Retrieve parameter information
            const char *paramName = it->first;
            ParameterCallback paramCallback = it->second;

            // Check if the parameter exists within the list of params received
            cJSON *param = cJSON_DetachItemFromObject( params, paramName );
            if( param == nullptr ) {
                // Parameter NOT found, report it and set error
                LOG_DEBUG( "Required parameter missing: %s", paramName );
                error = Error::Type::PARAM_MISSING;
            } else {
                // Parameter found, perform callback to handle data and
                // propogate error
                error = paramCallback( param );
            }

        }

        return error;
    }

private:
    T *mControlObject;

};

}

#endif // FAWKES_COMMAND_TEMPLATE_H
