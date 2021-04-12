#include "fawkes/http/router.h"

namespace Fawkes
{

namespace Http
{

Router::Router()
{
}

void Router::addRoute( const char *path, const char *method, Route::Action action )
{
    Route route( path, method, action );
    mRoutes.push_back( route );
}

bool Router::processRequest( Http::Request &request, Http::Response &response )
{
    bool processed = false;

    const char *method = request.method();
    const char *path = request.path();

    bool pathOk = false;
    bool methodOk = false;
    Route *route = nullptr;

    for( auto it = mRoutes.begin(); (it != mRoutes.end()) && !pathOk; it++ ) {
        if( strncmp( path, it->path(), HTTP_URL_LENGTH_MAX ) == 0 ) {
            if( strcmp( method, it->method() ) == 0 ) {
                methodOk = true;
            }
            pathOk = true;
            route = &(*it);
        }
    }

    if( !pathOk ) {
        LOG_INFO( "Invalid Path" );
    } else {
        if( methodOk ) {
            Http::Route::Action action = route->action();
            action( &request, &response );
            processed = true;
        }
    }

    return processed;
}

}

}
