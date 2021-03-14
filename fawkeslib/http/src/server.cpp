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
    HttpRequest *header = static_cast< HttpRequest* >( cls );
    // Insert the new header
    header->addHeader( key, value );

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
    printf( "%s\n", __FUNCTION__ );
    // Unused
    (void)( kind );
    (void)( off );

    HttpRequest *r = static_cast< HttpRequest* >( coninfo_cls );

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
    HttpRequest *request = static_cast< HttpRequest* >( *con_cls );

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

void HttpServer::onResponseSent(
        void *cls
        , MHD_Connection *connection
        , void **request
        , MHD_RequestTerminationCode *rtc)
{
    // printf( "Finished Request\n" );
    // Unused
    (void)cls;
    (void)connection;
    (void)rtc;

    HttpRequest *r = static_cast< HttpRequest* >( *request );

    if( r->mPostProcessor != nullptr ) {
        MHD_destroy_post_processor( r->mPostProcessor );
    }

    if( r->mFp ) {
        fclose( r->mFp );
    }

    // Delete the data contained within the request
    delete[] r->mData;

    // Delete the request
    delete r;
}

int HttpServer::onRequest( MHD_Connection *connection
        , const char *method
        , const char *path
        , void **request )
{
    // Create a new requst to handle the connection
    HttpRequest *r = new HttpRequest( connection );

    // Parse the header value
    MHD_get_connection_values(
                connection
                , MHD_HEADER_KIND
                , &iterateHeaderValues
                , r );

    // Create a new post processor
    r->mPostProcessor = MHD_create_post_processor(
                connection
                , 4096
                , iteratePost
                , static_cast< void* >( r ) );

    // A NULL post processor means we will have to handle it ourselves
    if( r->mPostProcessor == nullptr ) {
        // if( isVerbose() ) { printf( "PostProcessor is NULL\n" ); }
    }

    // Fill out the rest of the request
    r->setMethod( method );
    r->setPath( path );
    // r->mFp       = nullptr;
    // r->mData     = nullptr;
    // r->mDataSize = 0;

    *request = r;

    return MHD_YES;
}

int HttpServer::onRequestDone( HttpRequest *request )
{
    // processRequest( request );
    process( request );
    return MHD_YES;
}


int HttpServer::onRequestBody(
        HttpRequest *request
        , const char *data
        , size_t *size )
{
    request->appendData( data, *size );

    *size = 0;
    return MHD_YES;
}

void HttpServer::process( HttpRequest *request )
{
    // printHeaders( request );
    // printBody( request );

    const char *rspData = response_bad_request;
    const char *rspType = type_text_html;
    int rspCode   = MHD_HTTP_BAD_REQUEST;

    // In case we get a response string for the post
    char *rspStr = nullptr;

    // if( strcmp( request->mMethod, MHD_HTTP_METHOD_GET ) == 0 ) {
    //     // Handles a GET request

    //     if( strcmp( request->getPath(), path_base ) == 0
    //             || strcmp( request->getPath(), mIndexHtml ) == 0 ) {
    //         // The index has been requested
    //         rspData = mIndexHtml;
    //         rspType = type_text_html;
    //         rspCode = MHD_HTTP_OK;

    //     } else if( strcmp( request->getPath(), path_bundle_js ) == 0 ) {
    //         // The main script file has been requested
    //         rspData = mMainJs;
    //         rspType = type_text_javascript;
    //         rspCode = MHD_HTTP_OK;
    //     }

    // } else if ( strcmp( request->getMethod(), MHD_HTTP_METHOD_POST ) == 0 ) {
    if ( strcmp( request->getMethod(), MHD_HTTP_METHOD_POST ) == 0 ) {
        // Handles a POST request

        // if( request->mPostProcessor != nullptr ) {
        //     // A POST processor exists
        //     int ret = MHD_post_process( request->mPostProcessor
        //                       , request->getBody()->getData()
        //                       , request->getBody()->getSize() );

        //     if( ret == MHD_YES ) {
        //         rspData = response_success;
        //     } else {
        //         rspData = response_failed;
        //     }

        //     rspType = type_text_html;
        //     rspCode = MHD_HTTP_OK;
        // } else if( mCommandHandler != nullptr ){
        char *response = nullptr;
        int32_t value = 0;
        if( mCommandCallback ) {
            value = mCommandCallback( request->getBody()->data(), &response );

            if( response == nullptr ) {
                LOG_WARN( "Command response is NULL" );
            } else {
                rspStr = response;
                rspData = rspStr;
                rspType = type_text_html;
                rspCode = MHD_HTTP_OK;

                request->sendResponse( rspData, rspType, rspCode );
            }

            if( response ) {
                free( response );
            }
        }
    }
}

/**
 * @brief HttpServer::HttpServer Constructor
 */
HttpServer::HttpServer()
    : mPort( 8080 )
    , mDaemon( nullptr )
{
    applyCommandCallback( std::bind( &HttpServer::defaultHandler
                                     , this, std::placeholders::_1, std::placeholders::_2 ) );
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
                , nullptr
                , nullptr
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
}
