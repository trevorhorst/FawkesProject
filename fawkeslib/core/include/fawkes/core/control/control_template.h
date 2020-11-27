#ifndef FAWKES_CONTROL_TEMPLATE_H
#define FAWKES_CONTROL_TEMPLATE_H

#include <vector>

#include "fawkes/core/control/control.h"

namespace Fawkes
{

template< typename T >
class ControlTemplate : public Control
{
public:

    /**
     * @brief Constructor
     */
    ControlTemplate()
        : Control()
    {
        objects.push_back( static_cast< T* >( this ) );
    }

    /**
     * @brief Destructor
     */
    ~ControlTemplate()
    {
        // for( std::vector< T* >::const_iter it = objects.begin(); it != objects.end(); it++ ) {
        // }
    }

    /**
     * @brief Retrieves the number of available control objects
     * @return
     */
    static uint32_t count()
    {
        return objects.size();
    }

    static T *at( uint32_t id )
    {
        T *obj = nullptr;
        if( id < objects.size() ) {
            obj = objects.at( id );
        }
        return obj;
    }

private:
    static std::vector< T* > objects;
    uint32_t mId;
};


template< typename T >
std::vector< T* > ControlTemplate< T >::objects = {};

}

#endif // FAWKES_CONTROL_TEMPLATE_H
