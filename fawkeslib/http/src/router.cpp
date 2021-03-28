#include "fawkes/http/router.h"

namespace Fawkes
{

namespace Http
{

Router::Router()
{
}

void Router::addRoute(const char *path, const char *method, Route::Action action)
{
    Route route( path, method, action );
    mRoutes.push_back( route );
}

void Router::processRequest( HttpRequest &request )
{
    const char *method = request.method();
    const char *path = request.path();

    bool found = false;
    Route *route = nullptr;

    for( auto it = mRoutes.begin(); (it != mRoutes.end()) && !found; it++ ) {
        if( strncmp( path, it->path(), HTTP_URL_LENGTH_MAX ) == 0 ) {
            found = true;
            route = &(*it);
        }
    }

    if( !found ) {
        LOG_INFO( "Route not found" );
    } else {
        Http::Response rsp( request.connection() );
        Http::Route::Action action = route->action();
        action( &request, &rsp );
    }
}

}

}
