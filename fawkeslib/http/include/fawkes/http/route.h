#ifndef FAWKES_HTTP_ROUTE_H
#define FAWKES_HTTP_ROUTE_H

#include <functional>

#include "fawkes/core/control/control.h"
#include "fawkes/http/http.h"

#include "fawkes/http/request.h"
#include "fawkes/http/response.h"

namespace Fawkes
{

namespace Http
{

class Route
{
public:
    using Action = std::function< void (HttpRequest*, Http::Response*) >;

    Route( const char *path, const char *method, Action action );

    const char *method();
    const char *path();
    Action action();

    void applyPath( const char *path );
    void applyMethod( const char *method );
    void applyAction( Action action );

private:
    /// @note
    char mPath[ HTTP_URL_LENGTH_MAX ];
    const char *mMethod;
    Action mAction;
};

}

}

#endif // FAWKES_HTTP_ROUTE_H
