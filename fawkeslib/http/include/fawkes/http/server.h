#ifndef FAWKES_HTTP_SERVER_H
#define FAWKES_HTTP_SERVER_H

#include <functional>
#include <microhttpd.h>

#include "fawkes/core/transport/server.h"

#include "fawkes/http/request.h"

namespace Fawkes
{

class HttpServer
        : public Server
{
    static const char *response_success;
    static const char *response_failed;
    static const char *response_bad_request;

    static const char *type_text_html;
    static const char *type_text_javascript;

    static const char *path_base;
    static const char *path_index_html;
    static const char *path_bundle_js;

public:
    using CommandCallback = std::function< int32_t ( const char *, char ** ) >;

    HttpServer();
    virtual ~HttpServer();

    static int iterateHeaderValues(
            void *cls
            , enum MHD_ValueKind kind
            , const char *key
            , const char *value);

    static int iteratePost(
            void *coninfo_cls
            , MHD_ValueKind kind
            , const char *key
            , const char *filename
            , const char *content_type
            , const char *transfer_encoding
            , const char *data
            , uint64_t off
            , size_t size );

    static int32_t onConnection(
            void *cls
            , MHD_Connection *connection
            , const char *url, const char *method
            , const char *version, const char *upload_data
            , size_t *upload_data_size
            , void **con_cls
            );

    static void onResponseSent(
            void *cls
            , MHD_Connection *connection
            , void **request
            , MHD_RequestTerminationCode *rtc
            );

    int onRequest(MHD_Connection *connection
            , const char *method
            , const char *path
            , void **request );

    int onRequestBody(HttpRequest *request
            , const char *data
            , size_t *size );

    int onRequestDone(
            HttpRequest *request );

    void process( HttpRequest *request );

    void stop();
    int32_t listen() override;

    int32_t applyCommandCallback( CommandCallback callback );

private:
    uint16_t mPort;
    MHD_Daemon *mDaemon;
    CommandCallback mCommandCallback;

    int32_t defaultHandler( const char *data, char **response );
};

}

#endif // FAWKES_HTTP_SERVER_H
