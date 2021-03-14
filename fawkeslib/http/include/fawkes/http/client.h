#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <vector>
#include <curl/curl.h>

#include "fawkes/core/transport/client.h"

#include "fawkes/http/http.h"

namespace Fawkes {

class HttpClient
        : public Client
{

public:

    struct Data {
        Data();
        ~Data();
        const char* read();
        void write( const char *data, size_t size );
        void clear();
        char *raw();
        size_t size();
    private:
        char *mData;
        size_t mSize;
    };

    using WriteFunction = size_t ( void *, size_t, size_t, std::string* );

    HttpClient( const char *address = "127.0.0.1"
            , uint16_t port = 8080 );
    ~HttpClient();

    void close();
    uint32_t clearHeaders();

    std::string get();

    void send( const char *str );
    int32_t send( const char *data, char *response, int32_t responseLength ) override;

    uint32_t applyUrl( const std::string &url );
    uint32_t applyHeaders( const std::vector< std::string > &headers );
    uint32_t applyWriteFunction( WriteFunction *writeFunction );
    uint32_t applyBuffer( Data &buffer );

    static WriteFunction writeFunction;

private:
    CURL *mCurl;
    std::string mData;
    char mUrl[ 2048 ];

    curl_slist *mHeaders;

    uint16_t mPort;

    char *mResponse;
    uint16_t mResponseSize;
};

}

#endif // HTTP_CLIENT_H
