#include <string.h>

#include "fawkes/core/resource.h"

namespace Fawkes
{

Resource::Resource( const char *resource, const unsigned int resourceSize )
    : mResource( nullptr )
{
    load( resource, resourceSize );
}

/**
 * @brief Loads a resource file
 * @param resource Resource file to load
 * @param size Size, in bytes, of the resource we are loading
 * @return Pointer to the resource
 */
void Resource::load( const char *resource, unsigned int size )
{
    char *mResource = new char[ size + 1 ];
    memcpy( mResource, resource, size );
    mResource[ size ] = '\0';
}


/**
 * @brief Unloads a resource file
 * @param resource Resource file to unload
 */
void Resource::unload()
{
    if( mResource ) {
        delete[] mResource;
    }
}


}
