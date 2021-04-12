#include "fawkes/http/request.h"

namespace Fawkes {

namespace Http {

const uint32_t Request::header_key_size_max   = 128;
const uint32_t Request::header_value_size_max = 1024;

int32_t Request::iteratePost(
        void *coninfo
        , MHD_ValueKind kind
        , const char *key
        , const char *filename
        , const char *contentType
        , const char *transferEncoding
        , const char *data
        , uint64_t off, size_t size)
{
    // Unused
    (void)( kind );
    (void)( off );

    Request *request = static_cast< Request* >( coninfo );

    if( key != nullptr ) {
        LOG_INFO( "Key: %s", key );
    }

    if( contentType != nullptr ) {
        LOG_INFO( "Content-Type: %s", contentType );
    }

    if( transferEncoding != nullptr ) {
        LOG_INFO( "Transfer-Encoding: %s", transferEncoding );
    }

    if( filename == nullptr ) {
        LOG_ERROR( "Filename is invalid, not sure what to do in this case" );
    } else {
        if( request->mFp == nullptr ) {
            request->mFp = fopen( filename, "wb" );
            if( !request->mFp ) {
                return MHD_NO;
            }
        }
    }

    if( size > 0 ) {
        if( !fwrite( data, size, sizeof( char ), request->mFp ) ) {
            return MHD_NO;
        }
    }

    return MHD_YES;
}

/**
 * @brief Constructor
 * @param connection Connections associated with the request
 */
Request::Request( MHD_Connection *connection )
  : mConnection( connection )
  , mPostProcessor( nullptr )
  , mFp( nullptr )
  , mMethod( nullptr )
  , mPath( nullptr )
{
    /// @note Currently, libmicrohttp only supports the following:
    ///  application/x-www-form-urlencoded
    ///  multipart/form-data
    mPostProcessor = MHD_create_post_processor(
                connection
                , 4096
                , iteratePost
                , static_cast< void* >( this )
                );

    if( mPostProcessor == nullptr ) {
        LOG_TRACE( "Post processor is NULL" );
    }
}

/**
 * @brief Destructor
 */
Request::~Request()
{
    if( mPostProcessor ) {
        MHD_destroy_post_processor( mPostProcessor );
        mPostProcessor = nullptr;
    }

    if( mFp ) {
        fclose( mFp );
    }
}

/**
 * @brief Adds a header to the request
 * @param key Header key
 * @param value Header value
 */
void Request::addHeader( const char *key, const char *value )
{
    mHeaders[ key ] = value;
}

/**
 * @brief Append data to the request
 * @param data Data to append
 * @param size Size of the data to append
 */
void Request::appendData( const char *data, size_t size )
{
    mBody.append( data, size );
}

/**
 * @brief Send a response to the incoming request
 * @param responseData Data to send
 * @param responseType Type of response
 * @param statusCode Status code in relation to the response
 * @return Integer indicating the success of the operation
 */
int Request::sendResponse( const char *responseData, const char *responseType
                           , int statusCode )
{
    int ret;

    struct MHD_Response *response = nullptr;
    response = MHD_create_response_from_buffer(
                strlen( responseData )
                , (void*)( responseData )
                , MHD_RESPMEM_MUST_COPY );

    if( !response ) {
        return MHD_NO;
    }
    MHD_add_response_header( response
                             , MHD_HTTP_HEADER_CONTENT_TYPE
                             , responseType );
    ret = MHD_queue_response( mConnection, statusCode, response );

    MHD_destroy_response( response );

    return ret;
}

void Request::setPostProcessor( MHD_PostProcessor *processor )
{
    mPostProcessor = processor;
}

/**
 * @brief Sets the method type of the request
 * @param method Desired method type
 */
void Request::setMethod( const char *method )
{
    mMethod = method;
}

/**
 * @brief Sets the path of the request
 * @param path Desired path type
 */
void Request::setPath( const char *path )
{
    mPath = path;
}

/**
 * @brief Retrieves the method type of the request
 * @return Character array representation of the method type
 */
const char *Request::method()
{
    return mMethod;
}

/**
 * @brief Retrieves the path type of the request
 * @return Character array representation of the path
 */
const char *Request::path()
{
    return mPath;
}

/**
 * @brief Retrieves the body of the request
 * @return
 */
ByteArray *Request::body()
{
    return &mBody;
}

/**
 * @brief Retrieves the headers of the request
 * @return Pointer to the header map of the request
 */
const Request::HeaderMap *Request::headers()
{
    return &mHeaders;
}

MHD_Connection *Request::connection()
{
    return mConnection;
}

MHD_PostProcessor *Request::postProcessor()
{
    return mPostProcessor;
}

}

}
