#ifndef FAWKES_HTTP_REQUEST_H
#define FAWKES_HTTP_REQUEST_H

#include <microhttpd.h>

#include "fawkes/core/types/byte_array.h"
#include "fawkes/http/http.h"

namespace Fawkes
{

namespace Http
{

class Response
{
public:
    explicit Response( MHD_Connection *connection );

    void send( const char *data, size_t size );
    void send();

    void applyStatus( Http::Status status );
    void addHeader( const char *key, const char *value );

private:
    bool mSent;
    Http::Status mStatus;
    MHD_Connection *mConnection;
    Http::HeaderMap mHeaders;
    ByteArray mBody;
};

}

}

#endif // FAWKES_HTTP_REQUEST_H
