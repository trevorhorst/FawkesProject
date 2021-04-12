#ifndef FAWKES_HTTP_ROUTER_H
#define FAWKES_HTTP_ROUTER_H

#include <vector>

#include "fawkes/http/request.h"
#include "fawkes/http/route.h"

namespace Fawkes
{

namespace Http
{

class Router
{
public:
    Router();

    void addRoute( const char *path, const char * method, Route::Action action );
    bool processRequest( Http::Request &request, Http::Response &response );

private:
    std::vector< Route > mRoutes;

};

}

}

#endif // FAWKES_HTTP_ROUTER_HTTP
