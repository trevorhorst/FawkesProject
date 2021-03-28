/** ****************************************************************************
 * @file request.h
 * @author Trevor Horst
 * @copyright
 * @brief HttpRequest class declaration. Container for all information related to
 * a request. This class should not actually create any data.
 * ****************************************************************************/

#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <microhttpd.h>

#include "fawkes/core/common_types.h"
#include "fawkes/core/logger/log.h"
#include "fawkes/core/types/byte_array.h"
#include "fawkes/http/http.h"

namespace Fawkes {

class HttpRequest
{
public:

    using HeaderMap = Types::CharHashMap< const char* >;

    explicit HttpRequest( MHD_Connection *connection );
    ~HttpRequest();

    FILE* mFp;

    void setPostProcessor( MHD_PostProcessor *processor );
    void setMethod( const char *method );
    void setPath( const char *path );

    const char *method();
    const char *path();
    ByteArray *body();
    const HeaderMap *headers();
    MHD_Connection *connection();
    MHD_PostProcessor *postProcessor();

    void addHeader( const char *key, const char *value );
    void appendData( const char *data, size_t size );

    int sendResponse( const char *responseData
                      , const char *responseType
                      , int statusCode );

private:
    MHD_Connection *mConnection;
    MHD_PostProcessor *mPostProcessor;
    const char *mMethod;
    const char *mPath;

    HeaderMap mHeaders;
    ByteArray mBody;

    static const uint32_t header_key_size_max;
    static const uint32_t header_value_size_max;
};

}

#endif // HTTP_REQUEST_H
