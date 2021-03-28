#include "fawkes/http/response.h"

namespace Fawkes
{

namespace Http
{

/**
 * @brief Response::Response Response object
 * @param connection
 */
Response::Response( MHD_Connection *connection )
    : mSent( false )
    , mStatus( Http::Status::BADREQUEST )
    , mConnection( connection )
{

}

void Response::send(const char *data, size_t size)
{
    if( !mSent ) {
        mBody.append( data, size );
        send();
    }
}

void Response::send()
{
    struct MHD_Response *response = nullptr;
    response = MHD_create_response_from_buffer( mBody.size()
                                                , (void*)mBody.data()
                                                , MHD_RESPMEM_MUST_COPY );

    for( auto it = mHeaders.begin(); it != mHeaders.end(); it++ ) {
        MHD_add_response_header( response, (*it).first, (*it).second );
    }

    MHD_queue_response( mConnection, mStatus, response );
    MHD_destroy_response( response );
}

void Response::addHeader( const char *key, const char *value )
{
    mHeaders[ key ] = value;
}

void Response::applyStatus(Http::Status status)
{
    if( !mSent ) {
        mStatus = status;
    }
}

}

}
