#ifndef FAWKES_CORE_RESOURCE_H
#define FAWKES_CORE_RESOURCE_H

namespace Fawkes {

class Resource
{
public:
    Resource( const char *resource, const unsigned int resourceSize );
    ~Resource();

private:

    const char *mResource;

    void load( const char *resource, const unsigned int resourceSize );
    void unload();
};

}

#endif // FAWKES_CORE_RESOURCE_H
