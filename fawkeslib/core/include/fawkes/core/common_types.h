#ifndef FAWKES_COMMON_TYPES_H
#define FAWKES_COMMON_TYPES_H

#include <map>
#include <unordered_map>
#include <string.h>

namespace Fawkes
{

namespace Types {

struct CharArrayComparator
{
    bool operator()( const char *a, const char *b ) const;
};

struct CharArrayHash
{
    u_long operator()( const char *str ) const;
};

struct CharArrayHashComparator
{
    bool operator()( const char *a, const char *b ) const;
};

template< class T >
using CharHashMap = std::unordered_map< const char*, T, CharArrayHash, CharArrayHashComparator >;

template< class T >
using CharMap = std::map< const char*, T, CharArrayComparator >;

}

}

#endif // FAWKES_COMMON_TYPES_H
