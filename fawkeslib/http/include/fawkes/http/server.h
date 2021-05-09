#ifndef FAWKES_HTTP_SERVER_H
#define FAWKES_HTTP_SERVER_H

#include <functional>
#include <microhttpd.h>

#include "fawkes/core/transport/server.h"

#include "fawkes/http/request.h"
#include "fawkes/http/response.h"
#include "fawkes/http/router.h"

namespace Fawkes
{

namespace Http {

class Server
        : public ServerTemplate
{
public:
    static const char *response_success;
    static const char *response_failed;
    static const char *response_bad_request;

    static const char *type_text_html;
    static const char *type_text_javascript;

    static const char *path_base;
    static const char *path_index_html;
    static const char *path_bundle_js;

    using CommandCallback = std::function< int32_t ( const char *, char ** ) >;

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

    Server();
    virtual ~Server();

    int32_t onRequest(MHD_Connection *connection
            , const char *method
            , const char *path
            , void **request );

    int32_t onRequestBody(Http::Request *request
            , const char *data
            , size_t *size );

    int32_t onRequestDone(
            Http::Request *request );

    void process( Http::Request *request );

    void defaultAction( Http::Request *request, Http::Response *response );

    void stop();
    int32_t listen() override;

    int32_t applyCommandCallback( CommandCallback callback );
    int32_t applyRouter( Http::Router &router );

    int32_t defaultHandler( const char *data, char **response );

private:
    uint16_t mPort;
    MHD_Daemon *mDaemon;
    CommandCallback mCommandCallback;
    Http::Router mRouter;
};

}

}

#endif // FAWKES_HTTP_SERVER_H
