#include <cstdlib>
#include <string.h>

#include "fawkes/core/json/cjson.h"

#include "fawkes/http/client.h"

namespace Fawkes {

/**
 * @brief Constructor
 */
HttpClient::Data::Data()
    : mData( nullptr )
    , mSize( 0 )
{
}

/**
 * @brief Destructor
 */
HttpClient::Data::~Data()
{
    clear();
}

/**
 * @brief Read what has been written to the data block
 * @return Pointer to the data block
 */
const char *HttpClient::Data::read()
{
    return mData;
}

/**
 * @brief Writes to our data block
 * @param data Data to write
 * @param size Size of the data to write
 */
void HttpClient::Data::write( const char *data, size_t size )
{
    // Increase the size of our data
    mSize += size;
    // Save off a pointer to the old data
    char *t = mData;
    // Create a new block of memory
    mData = new char[ mSize + 1 ];
    // Copy the old data to the new block
    memcpy( static_cast< void* >( mData ), t, mSize - size );
    // Copy the new data to the end of the new block
    char *n = &mData[ mSize - size ];
    memcpy( static_cast< void* >( n ), data, size );
    // Append the null character
    mData[ mSize ] = '\0';

    if( t ) {
        // Delete the pointer to the old data block
        delete[] t;
    }
}

/**
 * @brief Clears the data
 */
void HttpClient::Data::clear()
{
    if( mData ) {
        mSize = 0;
        delete[] mData;
        mData = nullptr;
    }
}

char *HttpClient::Data::raw()
{
    return mData;
}

size_t HttpClient::Data::size()
{
    return mSize;
}

/**
 * @brief Constructor
 * @param address Address to
 * @param port
 */
HttpClient::HttpClient( const char *address , uint16_t port )
    : mCurl( nullptr )
    , mHeaders( nullptr )
    , mPort( port )
    , mResponse( nullptr )
    , mResponseSize( 0 )
{
    mCurl = curl_easy_init();

    // Default cURL settings
    applyUrl("localhost:8080");
    // applyUrl( "https://postman-echo.com/get?foo1=bar1&foo2=bar2" );
    // curl_easy_setopt(mCurl, CURLOPT_USERPWD, "user:pass");
    curl_easy_setopt( mCurl, CURLOPT_NOPROGRESS, 1L );
    curl_easy_setopt( mCurl, CURLOPT_USERAGENT, "curl/7.42.0" );
    curl_easy_setopt( mCurl, CURLOPT_MAXREDIRS, 50L );
    curl_easy_setopt( mCurl, CURLOPT_TCP_KEEPALIVE, 1L );

    applyWriteFunction( &writeFunction );

}

/**
 * @brief Destructor
 */
HttpClient::~HttpClient()
{
    close();
    mData.clear();
}

/**
 * @brief Cleans up the HTTP connection
 */
void HttpClient::close()
{
    clearHeaders();
    curl_easy_cleanup( mCurl );
    mCurl = nullptr;
}

/**
 * @brief Cleans up the headers list
 * @return Error code
 */
uint32_t HttpClient::clearHeaders()
{
    uint32_t error = Error::Type::NONE;
    if( mHeaders ) {
        curl_slist_free_all( mHeaders );
        mHeaders = nullptr;
    }
    return error;
}

/**
 * @brief Applies the URL to the cURL object
 * @param url Reference to the desired server
 * @return Error code
 */
uint32_t HttpClient::applyUrl( const std::string &url )
{
    uint32_t error = Error::Type::NONE;
    curl_easy_setopt( mCurl, CURLOPT_URL, url.c_str() );
    return error;
}

/**
 * @brief Applies headers to the cURL object
 * @param headers Reference to the list of desired headers
 * @return Error code
 */
uint32_t HttpClient::applyHeaders( const std::vector<std::string> &headers )
{
    uint32_t error = Error::Type::NONE;

    for( auto i = headers.begin(); i != headers.end(); i++ ) {
        mHeaders = curl_slist_append( mHeaders, (*i).c_str() );
    }

    curl_easy_setopt( mCurl, CURLOPT_MAIL_RCPT, mHeaders );

    return error;
}

/**
 * @brief Applies the method for writing data back
 * @param writeFunction Pointer to the desired write function
 * @return Error code
 */
uint32_t HttpClient::applyWriteFunction( WriteFunction *writeFunction )
{
    uint32_t error = Error::Type::NONE;
    curl_easy_setopt( mCurl, CURLOPT_WRITEFUNCTION, *writeFunction );
    return error;
}

/**
 * @brief Receives data from a connection
 * @param ptr Pointer to the data received
 * @param size Size is always 1
 * @param nmemb Size of the data received
 * @param d Pointer to the existing data
 * @return
 */
size_t HttpClient::writeFunction(
        void *ptr, size_t size, size_t nmemb, std::string *data )
{
    data->append( (char*)ptr, size *nmemb );
    return size * nmemb;
}

/**
 * @brief Performs a GET request
 * @return Response string
 */
std::string HttpClient::get()
{
    std::string response_string;
    if( mCurl ) {
        std::string header_string;
        curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(mCurl, CURLOPT_HEADERDATA, &header_string);

        char* url;
        long response_code;
        double elapsed;
        curl_easy_getinfo(mCurl, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_getinfo(mCurl, CURLINFO_TOTAL_TIME, &elapsed);
        curl_easy_getinfo(mCurl, CURLINFO_EFFECTIVE_URL, &url);

        curl_easy_perform(mCurl);
    }

    return response_string;
}

/**
 * @brief Sends a message
 * @param str Message to send
 */
int32_t HttpClient::send( const char *str, char *response, int32_t responseLength )
{
    LOG_INFO("Sending: %s", str);

    // Create the headers
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append( headers, "Content-Type: application/json" );
    headers = curl_slist_append( headers, "charset=utf-8" );

    // Clear the response
    mData.clear();

    // Set the options for cURL
    curl_easy_setopt( mCurl,           CURLOPT_URL, "http://127.0.0.1:8080" );
    curl_easy_setopt( mCurl,    CURLOPT_HTTPHEADER, headers );
    curl_easy_setopt( mCurl,  CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1 );
    curl_easy_setopt( mCurl,    CURLOPT_POSTFIELDS, str );
    curl_easy_setopt( mCurl,      CURLOPT_NOSIGNAL, 0 );
    curl_easy_setopt( mCurl, CURLOPT_WRITEFUNCTION, HttpClient::writeFunction );
    curl_easy_setopt( mCurl,     CURLOPT_WRITEDATA, &mData );

    // Set up for cookie usage
    curl_easy_setopt( mCurl, CURLOPT_COOKIESESSION, true );
    curl_easy_setopt( mCurl,     CURLOPT_COOKIEJAR, "cs-cookie" );
    curl_easy_setopt( mCurl,    CURLOPT_COOKIEFILE, "/tmp" );

    // Perform the cURL
    CURLcode res = curl_easy_perform( mCurl );

    if( res != CURLE_OK ) {
        // cURL failed
        fprintf( stderr, "curl_easy_perform() failed: %s\n",
                      curl_easy_strerror( res ) );
    } else {
        // Handle the response
        strncpy(response, mData.raw(), responseLength);
    }

    // Free the headers
    curl_slist_free_all( headers );

    return Error::Type::NONE;
}

}
