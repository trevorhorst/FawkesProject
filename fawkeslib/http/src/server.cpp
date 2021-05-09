#include "fawkes/http/server.h"

namespace Fawkes {

const char *HttpServer::response_success     = "Success";
const char *HttpServer::response_failed      = "Failed";
const char *HttpServer::response_bad_request = "Bad Request";

const char *HttpServer::type_text_html       = "text/html";
const char *HttpServer::type_text_javascript = "text/javascript";

const char *HttpServer::path_base            = "/";
const char *HttpServer::path_index_html      = "/index.html";
const char *HttpServer::path_bundle_js         = "/bundle.js";

int HttpServer::iterateHeaderValues(
        void *cls
        , MHD_ValueKind kind
        , const char *key
        , const char *value )
{
    // Unused
    (void)kind;

    // Cast the existing headers map
    Http::Request *request = static_cast< Http::Request* >( cls );
    // Insert the new header
    request->addHeader( key, value );

    return MHD_YES;
}

int HttpServer::iteratePost(
        void *coninfo_cls
        , MHD_ValueKind kind
        , const char *key
        , const char *filename
        , const char *content_type
        , const char *transfer_encoding
        , const char *data
        , uint64_t off, size_t size )
{
    // Unused
    (void)( kind );
    (void)( off );

    Http::Request *r = static_cast< Http::Request* >( coninfo_cls );

    if( key != nullptr ) {
        printf( "Key: %s\n", key );
    }

    if( content_type != nullptr ) {
        printf( "Content-Type: %s\n", content_type );
    }

    if( transfer_encoding != nullptr ) {
        printf( "Transfer-Encoding: %s\n", transfer_encoding );
    }

    // con_info->answerstring = servererrorpage;
    // con_info->answercode = MHD_HTTP_INTERNAL_SERVER_ERROR;

    if( !r->mFp ) {
        // The file pointer is invalid
        // r->mFp = fopen( filename, "rb" );
        // if( !r->mFp ) {
        //     // We could read the file, which means it already exists
        //     fclose( r->mFp );
        //     // con_info->answerstring = fileexistspage;
        //     // con_info->answercode = MHD_HTTP_FORBIDDEN;
        //     return MHD_NO;
        // }

        // Just write over the file
        r->mFp = fopen( filename, "wb" );
        if( !r->mFp ) {
            // We failed to open the file for writing
            // con_info->answercode = MHD_HTTP_FORBIDDEN;
            return MHD_NO;
        }
    }

    if( size > 0 ) {
        if( !fwrite( data, size, sizeof(char), r->mFp ) ) {
            return MHD_NO;
        }
    }

    // con_info->answerstring = completepage;
    // con_info->answercode = MHD_HTTP_OK;

    return MHD_YES;
}


/**
 * @brief HttpServer::onConnection Handles any new connections received by the server
 * @param cls
 * @param connection Incoming connection
 * @param url The requested URL of the incoming connection
 * @param method The HTTP method used
 * @param version
 * @param upload_data
 * @param upload_data_size
 * @param con_cls
 * @return
 */
int32_t HttpServer::onConnection (
        void *cls
        , MHD_Connection *connection
        , const char *url, const char *method
        , const char *version, const char *upload_data
        , size_t *upload_data_size
        , void **con_cls
        )
{
    int32_t error = 0;
    // Unused
    ( void )( version );

    HttpServer *server = static_cast< HttpServer* >( cls );
    Http::Request *request = static_cast< Http::Request* >( *con_cls );

    if( request == nullptr ) {
        // Handles a new request
        error = server->onRequest( connection, method, url, con_cls );
    } else if( *upload_data_size > 0 ) {
        // Handles an existing request
        error = server->onRequestBody( request, upload_data, upload_data_size );
    } else {
        // Handles a finished request
        error = server->onRequestDone( request );
    }

    return error;
}

/**
 * @brief HttpServer::onResponseSent Cleans up a request once it has been handled
 * @param cls
 * @param connection Connection tied to the request object
 * @param request Container holding information regarding the request
 * @param rtc
 */
void HttpServer::onResponseSent(
        void *cls
        , MHD_Connection *connection
        , void **request
        , MHD_RequestTerminationCode *rtc)
{
    // Unused
    (void)cls;
    (void)connection;
    (void)rtc;

    Http::Request *r = static_cast< Http::Request* >( *request );

    // Delete the request
    delete r;
}

/**
 * @brief HttpServer::onRequest Handles a new incoming request
 * @param connection Connection associated with the request
 * @param method Method associated with the request
 * @param path Path associated with the request
 * @param request Container to store a newly created request object
 * @return int
 */
int32_t HttpServer::onRequest( MHD_Connection *connection
        , const char *method
        , const char *path
        , void **request )
{
    // Create a new requst to handle the connection
    Http::Request *r = new Http::Request( connection );

    // Parse the header value
    MHD_get_connection_values(
                connection
                , MHD_HEADER_KIND
                , &iterateHeaderValues
                , r );

    // // Create a new post processor
    // MHD_PostProcessor *processor = MHD_create_post_processor(
    //             connection
    //             , 4096
    //             , iteratePost
    //             , static_cast< void* >( r ) );

    // // A NULL post processor means we will have to handle it ourselves
    // if( processor == nullptr ) {
    //     LOG_TRACE( "Post processor is NULL" );
    // }

    // Fill out the rest of the request
    // r->setPostProcessor( processor );
    r->setMethod( method );
    r->setPath( path );
    // r->mFp       = nullptr;

    *request = r;

    return MHD_YES;
}

/**
 * @brief HttpServer::onRequestBody Handles the body of the request data
 * @param request Pointer to the request object
 * @param data Incoming request body data
 * @param size Size of the incoming request body data
 * @return
 */
int32_t HttpServer::onRequestBody(
        Http::Request *request
        , const char *data
        , size_t *size )
{
    request->appendData( data, *size );

    *size = 0;
    return MHD_YES;
}

/**
 * @brief HttpServer::onRequestDone Handles a completed request
 * @param request Pointer to the request object
 * @return int
 */
int32_t HttpServer::onRequestDone( Http::Request *request )
{
    process( request );
    return MHD_YES;
}


void HttpServer::process( Http::Request *request )
{
    const char *rspData = response_bad_request;
    const char *rspType = type_text_html;
    int rspCode   = MHD_HTTP_BAD_REQUEST;

    // In case we get a response string for the post
    char *rspStr = nullptr;

    Http::Response response( request->connection() );

    bool processed = false;

    processed = mRouter.processRequest( *request, response );

    if( processed ) {

    } else {
        response.applyStatus( Http::INTERNALSERVERERROR );
        response.send( "\0", 1 );
    }
}

/**
 * @brief HttpServer::HttpServer Constructor
 */
HttpServer::HttpServer()
    : mPort( 8080 )
    , mDaemon( nullptr )
{
    // applyCommandCallback( std::bind( &HttpServer::defaultHandler
    //                                  , this, std::placeholders::_1, std::placeholders::_2 ) );

    // mRouter.addRoute( "/", MHD_HTTP_METHOD_POST
    //                   , std::bind( &HttpServer::defaultAction, this, std::placeholders::_1, std::placeholders::_2 ) );
}

/**
 * @brief HttpServer::~HttpServer Destructor
 */
HttpServer::~HttpServer()
{
    stop();
}

int32_t HttpServer::defaultHandler( const char *data, char **response )
{
    int32_t error = Error::Type::NONE;

    LOG_INFO( "Default Handler: %s", data );
    (void)response;

    return error;
}

void HttpServer::stop()
{
    if( mDaemon != nullptr ) {
        LOG_INFO( "stopping server...\n" );
        MHD_stop_daemon( mDaemon );
        mDaemon = nullptr;
    }
}

int32_t HttpServer::listen()
{
    int32_t error = 0;

    uint flags = MHD_USE_POLL_INTERNALLY
            | MHD_USE_THREAD_PER_CONNECTION
            | MHD_USE_PEDANTIC_CHECKS;

    mDaemon = MHD_start_daemon(
                flags
                , mPort
                , nullptr, nullptr
                , &onConnection, this
                , MHD_OPTION_NOTIFY_COMPLETED, &onResponseSent, this
                // , MHD_OPTION_SOCK_ADDR, &socket
                , MHD_OPTION_END
                );

    if( mDaemon == nullptr ) {
        LOG_WARN( "server daemon failed to start\n" );
        error = -1;
    } else {
        LOG_INFO( "server daemon started successfully" );
        LOG_INFO( "listening on port %d", mPort );
    }

    return error;
}

int32_t HttpServer::applyCommandCallback( CommandCallback callback )
{
    int32_t error = Error::Type::NONE;
    mCommandCallback = callback;
    return error;
}

int32_t HttpServer::applyRouter( Http::Router &router )
{
    int32_t error = Error::Type::NONE;
    mRouter = router;
    return error;
}

void HttpServer::defaultAction( Http::Request *request, Http::Response *response )
{
    char *responseArray = nullptr;
    int32_t value = 0;
    if( mCommandCallback ) {
        value = mCommandCallback( request->body()->data(), &responseArray );

        if( responseArray == nullptr ) {
            LOG_WARN( "Command response is NULL" );
        } else {
            // Http::Response rsp( request->connection() );
            response->applyStatus( Http::OK );
            response->addHeader( MHD_HTTP_HEADER_CONTENT_TYPE, type_text_html );
            response->send( responseArray, strlen( responseArray ) );
        }

        if( responseArray ) {
            free( responseArray );
        }
    }
};

}
