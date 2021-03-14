#ifndef FAWKES_CORE_BYTE_ARRAY_H
#define FAWKES_CORE_BYTE_ARRAY_H

#include <stddef.h>
#include <string.h>

namespace Fawkes
{

class ByteArray
{
public:
    ByteArray();
    ~ByteArray();

    const char *data();
    size_t size();

    ByteArray &append( const char *data, size_t size );
    void clear();

private:
    char *mData;
    size_t mSize;
};

}

#endif // FAWKES_CORE_BYTE_ARRAY_H
