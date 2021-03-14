#include "fawkes/core/types/byte_array.h"

namespace Fawkes
{

ByteArray::ByteArray()
    : mData( nullptr )
    , mSize( 0 )
    , mCapacity( 0 )
{

}

ByteArray::ByteArray( const char *data, size_t size )
    : mData( nullptr )
    , mSize( size )
    , mCapacity( size )
{
    mData = new char[ mSize + 1 ];
    memcpy( static_cast< void* >( mData ), data, mSize );
    mData[ mSize ] = '\0';
}

ByteArray::~ByteArray()
{
    clear();
}

const char *ByteArray::data()
{
    return mData;
}

size_t ByteArray::size()
{
    return mSize;
}

ByteArray &ByteArray::append(const char *data, size_t size)
{
    // Increase the size of our data
    mSize += size;
    // Save off a pointer to the old data
    char *t = mData;
    // Create a new block of memory
    mData = new char[ mSize + 1 ];
    // Copy the old data to the new block
    memcpy( static_cast< void* >( mData ), t, mSize - size );
    // Copy the new data to the end of the new block
    char *n = &mData[ mSize - size ];
    memcpy( static_cast< void* >( n ), data, size );
    // Append the null character
    mData[ mSize ] = '\0';

    if( t ) {
        // Delete the pointer to the old data block
        delete[] t;
    }

    return *this;
}

void ByteArray::clear()
{
    if( mData ) {
        delete[] mData;
        mData = nullptr;
        mSize = 0;
    }
}

}
