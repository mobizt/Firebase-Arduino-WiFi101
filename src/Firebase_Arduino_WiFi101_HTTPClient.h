/*
 * HTTP Client Wrapper based on WiFi101 librarys, version 1.0.0
 *
 * 
 * This library required WiFi101 Library to be installed.
 * https://github.com/arduino-libraries/WiFi101
 * 
 * April 29, 2019
 * 
 * The MIT License (MIT)
 * Copyright (c) 2019 K. Suwatchai (Mobizt)
 * 
 * 
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef Firebase_Arduino_WiFi101_HTTPClient_H
#define Firebase_Arduino_WiFi101_HTTPClient_H

#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>

/// HTTP client errors
#define HTTPC_ERROR_CONNECTION_REFUSED (-1)
#define HTTPC_ERROR_SEND_HEADER_FAILED (-2)
#define HTTPC_ERROR_SEND_PAYLOAD_FAILED (-3)
#define HTTPC_ERROR_NOT_CONNECTED (-4)
#define HTTPC_ERROR_CONNECTION_LOST (-5)
#define HTTPC_ERROR_NO_STREAM (-6)
#define HTTPC_ERROR_NO_HTTP_SERVER (-7)
#define HTTPC_ERROR_TOO_LESS_RAM (-8)
#define HTTPC_ERROR_ENCODING (-9)
#define HTTPC_ERROR_STREAM_WRITE (-10)
#define HTTPC_ERROR_READ_TIMEOUT (-11)
#define FIREBASE_ERROR_BUFFER_OVERFLOW (-13)
#define FIREBASE_ERROR_DATA_TYPE_MISMATCH (-14)
#define FIREBASE_ERROR_PATH_NOT_EXIST (-15)

/// HTTP codes see RFC7231

#define _HTTP_CODE_OK 200
#define _HTTP_CODE_NON_AUTHORITATIVE_INFORMATION 203
#define _HTTP_CODE_NO_CONTENT 204
#define _HTTP_CODE_MOVED_PERMANENTLY 301
#define _HTTP_CODE_USE_PROXY 305
#define _HTTP_CODE_TEMPORARY_REDIRECT 307
#define _HTTP_CODE_PERMANENT_REDIRECT 308
#define _HTTP_CODE_BAD_REQUEST 400
#define _HTTP_CODE_UNAUTHORIZED 401
#define _HTTP_CODE_FORBIDDEN 403
#define _HTTP_CODE_NOT_FOUND 404
#define _HTTP_CODE_METHOD_NOT_ALLOWED 405
#define _HTTP_CODE_NOT_ACCEPTABLE 406
#define _HTTP_CODE_PROXY_AUTHENTICATION_REQUIRED 407
#define _HTTP_CODE_REQUEST_TIMEOUT 408
#define _HTTP_CODE_LENGTH_REQUIRED 411
#define _HTTP_CODE_PAYLOAD_TOO_LARGE 413
#define _HTTP_CODE_URI_TOO_LONG 414
#define _HTTP_CODE_MISDIRECTED_REQUEST 421
#define _HTTP_CODE_UNPROCESSABLE_ENTITY 422
#define _HTTP_CODE_TOO_MANY_REQUESTS 429
#define _HTTP_CODE_REQUEST_HEADER_FIELDS_TOO_LARGE 431
#define _HTTP_CODE_INTERNAL_SERVER_ERROR 500
#define _HTTP_CODE_NOT_IMPLEMENTED 501
#define _HTTP_CODE_BAD_GATEWAY 502
#define _HTTP_CODE_SERVICE_UNAVAILABLE 503
#define _HTTP_CODE_GATEWAY_TIMEOUT 504
#define _HTTP_CODE_HTTP_VERSION_NOT_SUPPORTED 505
#define _HTTP_CODE_LOOP_DETECTED 508
#define _HTTP_CODE_NETWORK_AUTHENTICATION_REQUIRED 511

#define _HOST_LENGTH 100

class FirebaseHTTPClient
{
public:
    FirebaseHTTPClient();
    ~FirebaseHTTPClient();

    bool http_begin(const char *host, uint16_t port);
    bool http_connected(void);
    int http_sendRequest(const char *header, const char *payload);
    WiFiSSLClient client;
    uint16_t netClientTimeout = 5000;

protected:
    bool http_connect(void);
    bool http_sendHeader(const char *header);
    char *_host = new char[_HOST_LENGTH];
    uint16_t _port = 0;
};

#endif /* Firebase_Arduino_WiFi101_HTTPClient_H */
