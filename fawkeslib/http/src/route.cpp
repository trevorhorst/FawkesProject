#include "fawkes/http/route.h"

namespace Fawkes
{

namespace Http {

Route::Route( const char *path, const char *method,  Action action )
    : mPath{ "\0" }
    , mMethod( nullptr )
    , mAction( nullptr )
{
    applyPath( path );
    applyMethod( method );
    applyAction( action );
}

const char *Route::path()
{
    return mPath;
}

const char *Route::method()
{
    return mMethod;
}

Route::Action Route::action()
{
    return mAction;
}

void Route::applyPath( const char *path )
{
    if( path != nullptr ) {
        strncpy( mPath, path, HTTP_URL_LENGTH_MAX );
    }
}

void Route::applyMethod( const char *method )
{
    mMethod = method;
}

void Route::applyAction( Action action )
{
    mAction = action;
}

}

}
