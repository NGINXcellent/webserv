/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactoryTests.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 20:01:35 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/07 18:56:35 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

#include "../../../include/http/HttpRequest.hpp"
#include "../../../include/http/HttpRequestFactory.hpp"
#include "../../../include/http/Server.hpp"
#include "../../../include/io/FileReader.hpp"
#include "../../../include/io/FileSystem.hpp"
#include "../../../src/http/HttpRequestFactory.cpp"
#include "../../../src/http/HttpParser.cpp"
#include "../../../src/io/FileSystem.cpp"


void testRequestLine(std::string requestMsg, int expectedStatusCode,
                     std::vector<s_locationConfig> locations) {
  (void)locations;
  HttpRequest *request = new HttpRequest();
  HttpRequestFactory::setupHeader(request, requestMsg);
  EXPECT_EQ(HttpRequestFactory::check(request), expectedStatusCode)
      << requestMsg;
  delete request;
}

TEST(RequestTests, BasicBehaviourTest) {
  std::vector<std::string> reqLines;

  reqLines.push_back(
      "GET localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  reqLines.push_back(
      "GET localhost:8080/index.html HTTP/1.0\n");
  reqLines.push_back(
      "HEAD localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  reqLines.push_back(
      "DEL localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");

  std::vector<s_locationConfig> locations;
  s_locationConfig locationOne;
  std::string page = "/index.html";
  locationOne.location = page;
  locations.push_back(locationOne);

  for (size_t i = 0; i < reqLines.size(); i++) {
    testRequestLine(reqLines[i].c_str(), 0, locations);
  }
}

TEST(LocationTests, basicLocationTests) {
    std::vector<s_locationConfig> configs;

    s_locationConfig config1;
    config1.location = "/";
    config1.index = "index.html";
    config1.allowed_method = {"GET", "POST"};
    configs.push_back(config1);

    s_locationConfig config2;
    config2.location = "/images";
    config2.root = "/var/www/images";
    config2.allowed_method = {"GET"};
    configs.push_back(config2);

    s_locationConfig config3;
    config3.location = "/dir";
    config3.root = "./../InputTests/test_files/fail";
    config3.index = "nestedServer.conf";
    config3.allowed_method = {"GET"};
    configs.push_back(config3);

    s_locationConfig config4;
    config4.location = "/images/zeroum";
    config4.root = "/var";
    config4.allowed_method = {"GET"};
    configs.push_back(config4);

    std::string buffer1 = "/";
    std::string buffer2 = "/images";
    std::string buffer3 = "/teste4/vamos/testar/aqui";
    std::string buffer4 = "/dir";
    std::string buffer5 = "/images/zeroum";
    HttpRequest request;

    // will not return index becaus there is no valid index file
    request.setResource(buffer1);
    HttpRequestFactory::findLocation(&request, configs);
    std::string result1 = request.getIndexPath();
    EXPECT_EQ(result1, "./");

    request.setResource(buffer2);
    HttpRequestFactory::findLocation(&request, configs);
    std::string result2 = request.getIndexPath();
    EXPECT_EQ(result2, "/var/www/images");

    request.setResource(buffer3);
    HttpRequestFactory::findLocation(&request, configs);
    std::string result3 = request.getIndexPath();
    EXPECT_EQ(result3, "./teste4/vamos/testar/aqui");

    request.setResource(buffer4);
    HttpRequestFactory::findLocation(&request, configs);
    std::string result4 = request.getIndexPath();
    EXPECT_EQ(result4, "./../InputTests/test_files/fail");

    request.setResource(buffer5);
    HttpRequestFactory::findLocation(&request, configs);
    std::string result5 = request.getIndexPath();
    EXPECT_EQ(result5, "/var");
}

TEST(RequestTests, RequestLineMethodTest) {
  std::vector<std::string> msgs;

  // invalid name formatting
  msgs.push_back(
      "get localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "hEad localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "HEAd localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "Post localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "poSt localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "pOST localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "pOSt localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "DEl localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "ReLEASE localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");

  std::vector<s_locationConfig> locations;
  s_locationConfig locationOne;
  locationOne.location = "/index.html";
  locations.push_back(locationOne);

  for (size_t i = 0; i < msgs.size(); i++) {
    testRequestLine(msgs[i].c_str(), 400, locations);
  }
}

TEST(RequestTests, RequestLineProtocolTest) {
  std::vector<std::string> msgs;

  // invalid name formatting
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/1.a\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/1,1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/1,0\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP1.0\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP10\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP11\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/1.1-\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/1.-1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/0.0\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/0.1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/0.10\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/0.11\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/0.12\nhost: localhost:8080\n");

  // wrong protocol name formatting
  msgs.push_back(
      "GET localhost:8080/index.html http/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html hTtp/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html httP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HttP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html FTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html TCP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html /1.1\nhost: localhost:8080\n");

  std::vector<s_locationConfig> locations;
  s_locationConfig locationOne;
  locationOne.location = "/index.html";
  locations.push_back(locationOne);

  for (size_t i = 0; i < msgs.size(); i++) {
    testRequestLine(msgs[i].c_str(), 400, locations);
  }
}

TEST(RequestTests, RequestMissingFieldsTest) {
  std::vector<std::string> msgs;

  msgs.push_back("GET localhost:8080/index.html \nhost: localhost:8080\n");
  msgs.push_back("localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back("GET HTTP/1.1\nhost: localhost:8080\n");

  std::vector<s_locationConfig> locations;
  s_locationConfig locationOne;
  locationOne.location = "/index.html";
  locations.push_back(locationOne);

  for (size_t i = 0; i < msgs.size(); i++) {
    testRequestLine(msgs[i].c_str(), 400, locations);
  }
}

TEST(RequestTests, RequestExtraSpacesTests) {
  std::vector<std::string> msgs;

  msgs.push_back(
      "\tGET localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "         GET localhost:8080/index.html HTTP/1.1\nhost: "
      "localhost:8080\n");
  msgs.push_back(
      "GET            localhost:8080/index.html HTTP/1.1\nhost: "
      "localhost:8080\n");
  msgs.push_back(
      "GET\t\t\tlocalhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET\t    localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html          HTTP/1.1\nhost: "
      "localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html\t HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html\t\t\tHTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/1.1       \nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/1.1\t\nhost: localhost:8080\n");

  std::vector<s_locationConfig> locations;
  s_locationConfig locationOne;
  locationOne.location = "/index.html";
  locations.push_back(locationOne);

  for (size_t i = 0; i < msgs.size(); i++) {
    testRequestLine(msgs[i].c_str(), 0, locations);
  }
}

TEST(RequestTests, HttpVersionNotSupportedTest) {
  std::vector<std::string> msgs;

  msgs.push_back(
      "GET localhost:8080/index.html HTTP/1.10\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/2.0\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/2.42\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/3.00\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/42.42\nhost: localhost:8080\n");
  msgs.push_back(
      "GET localhost:8080/index.html HTTP/100.00\nhost: localhost:8080\n");

  std::vector<s_locationConfig> locations;
  s_locationConfig locationOne;
  locationOne.location = "/index.html";
  locations.push_back(locationOne);

  for (size_t i = 0; i < msgs.size(); i++) {
    testRequestLine(msgs[i].c_str(), 505, locations);
  }
}

TEST(RequestTests, HttpVersionOnePointZeroTest) {
  std::vector<std::string> msgs;

  msgs.push_back("GET localhost:8080/index.html HTTP/1.0\nhost: localhost:8080\n");
  msgs.push_back("GET localhost:8080/index.html HTTP/1.0\nhost:\n");
  msgs.push_back("GET localhost:8080/index.html HTTP/1.0\n");

  std::vector<s_locationConfig> locations;
  s_locationConfig locationOne;
  locationOne.location = "/index.html";
  locations.push_back(locationOne);

  for (size_t i = 0; i < msgs.size(); i++) {
    testRequestLine(msgs[i].c_str(), 0, locations);
  }
}

TEST(RequestTests, HttpRequestAttributesFormat) {
  std::vector<s_locationConfig> locations;
  s_locationConfig locationOne;
  locationOne.location = "/index.html";
  locations.push_back(locationOne);

  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host: localhost:8080\n";
    request += "Connection: keep-alive\n";
    request += "Accept-Language: en-US,en\n";

    testRequestLine(request.c_str(), 0, locations);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "   host: localhost:8080\n";
    request += "Connection: keep-alive\n";
    request += "Accept-Language: en-US,en\n";

    testRequestLine(request.c_str(), 400, locations);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host: localhost:8080\n";
    request += "\tConnection: keep-alive\n";
    request += "Accept-Language: en-US,en\n";

    testRequestLine(request.c_str(), 400, locations);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host: localhost:8080\n";
    request += "Connection: keep-alive\n";
    request += "   \tAccept-Language: en-US,en\n";

    testRequestLine(request.c_str(), 400, locations);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host:       localhost:8080\n";

    testRequestLine(request.c_str(), 0, locations);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host:\t\tlocalhost:8080\n";

    testRequestLine(request.c_str(), 0, locations);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host:localhost:8080\n";

    testRequestLine(request.c_str(), 400, locations);
  }
}

TEST(RequestTests, CRLFParsingTest) {
  std::vector<s_locationConfig> locations;
  s_locationConfig locationOne;
  locationOne.location = "/index.html";
  locations.push_back(locationOne);

  {
    std::string request;
    request += "GET /index.html HTTP/1.1\r\n";
    request += "host: localhost:8080\r\n";
    request += "Connection: keep-alive\r\n";
    request += "Accept-Language: en-US,en\r\n";

    testRequestLine(request.c_str(), 0, locations);
  }

  {
    std::string request;
    request += "HEAD /index.html HTTP/1.1\r\n";
    request += "host: localhost:8080\r\n";
    request += "Connection: keep-alive\r\n";
    request += "Accept-Language: en-US,en\r\n";

    testRequestLine(request.c_str(), 0, locations);
  }

  {
    std::string request;
    request += "DEL /index.html HTTP/1.1\r\n";
    request += "host: localhost:8080\r\n";
    request += "Connection: keep-alive\r\n";
    request += "Accept-Language: en-US,en\r\n";

    testRequestLine(request.c_str(), 0, locations);
  }

  {
    std::string request;
    request += "HEAD /index.html HTTP/1.1\r\n";
    request += "host: localhost:8080\r\n";
    request += "Connection: keep-alive\r\n";
    request += "Accept-Language: en-US,en\r\n";

    testRequestLine(request.c_str(), 0, locations);
  }

  // mixed
  {
    std::string request;
    request += "HEAD /index.html HTTP/1.1\n";
    request += "host: localhost:8080\r\n";
    request += "Connection: keep-alive\n";
    request += "Accept-Language: en-US,en\r\n";

    testRequestLine(request.c_str(), 0, locations);
  }
}

/* WORK ON THIS CHECK HTTP REQUEST FACTORY FUNCTION
TEST(RequestTests, BlankLinesTest) {
  std::vector<s_locationConfig> locations;
  s_locationConfig locationOne;
  locationOne.location = "/index.html";
  locations.push_back(locationOne);

  {
    std::string request;
    request += "GET /index.html HTTP/1.1\r\n";
    request += "\n";
    request += "host: localhost:8080\r\n";
    request += "Connection: keep-alive\r\n";
    request += "Accept-Language: en-US,en\r\n";

    testRequestLine(request.c_str(), 400, locations);
  }

  {
    std::string request;
    request += "HEAD /index.html HTTP/1.1\r\n";
    request += "\n";
    request += "host: localhost:8080\r\n";
    request += "Connection: keep-alive\r\n";
    request += "Accept-Language: en-US,en\r\n";

    testRequestLine(request.c_str(), 400, locations);
  }

  {
    std::string request;
    request += "HEAD /index.html HTTP/1.1\r\n";
    request += "host: localhost:8080\r\n";
    request += "\n";
    request += "Connection: keep-alive\r\n";
    request += "\n";
    request += "Accept-Language: en-US,en\r\n";

    testRequestLine(request.c_str(), 400, locations);
  }

  {
    std::string request;
    request += "POST /index.html HTTP/1.1\r\n";
    request += "host: localhost:8080\r\n";
    request += "\n";
    request += "Connection: keep-alive\r\n";
    request += "\n";
    request += "Accept-Language: en-US,en\r\n";

    testRequestLine(request.c_str(), 0, locations);
  }

  {
    std::string request;
    request += "DEL /index.html HTTP/1.1\r\n";
    request += "host: localhost:8080\r\n";
    request += "\n";
    request += "Connection: keep-alive\r\n";
    request += "\n";
    request += "Accept-Language: en-US,en\r\n";

    testRequestLine(request.c_str(), 400, locations);
  }

  {
    std::string request;
    request += "DEL /index.html HTTP/1.1\r\n";
    request += "host: localhost:8080\r\n";
    request += "\n\n\n\n\n\n\n";
    request += "Connection: keep-alive\r\n";
    request += "\n";
    request += "Accept-Language: en-US,en\r\n";

    testRequestLine(request.c_str(), 400, locations);
  }

  {
    std::string request;
    request += "GET /index.html HTTP/1.1\r\n";
    request += "host: localhost:8080\r\n";
    request += "\n\n\n\n\n\n\n";

    testRequestLine(request.c_str(), 0, locations);
  }
}*/

TEST(RequestTests, TestChunkType) {
  {
    HttpRequest request;
    std::map<std::string, std::string> headers;
    headers["transfer-encoding"] = "chunked";

    PostType result = HttpRequestFactory::setupBodyContentType(&request, headers);

    EXPECT_EQ(result, Chunked);
  }

  {
    HttpRequest request;
    std::map<std::string, std::string> headers;
    headers["content-type"] = "multipart/form-data; boundary=myboundary123";

    PostType result = HttpRequestFactory::setupBodyContentType(&request, headers);

    EXPECT_EQ(result, Multipart);
    EXPECT_EQ(request.getBoundary(), "myboundary123");
  }

  {
    HttpRequest request;
    std::map<std::string, std::string> headers;

    PostType result = HttpRequestFactory::setupBodyContentType(&request, headers);

    EXPECT_EQ(result, None);
  }

  {
    HttpRequest request;
    std::map<std::string, std::string> headers;
    headers["content-type"] = "multipart/wrongform-data; boundary=myboundary123";

    PostType result = HttpRequestFactory::setupBodyContentType(&request, headers);

    EXPECT_EQ(result, None);
  }
  {
    HttpRequest request;
    std::map<std::string, std::string> headers;
    headers["transfer-encoding"] = "chuunked";

    PostType result = HttpRequestFactory::setupBodyContentType(&request, headers);

    EXPECT_EQ(result, None);
  }
}

TEST(BodySizeTests, LocationMaxBodySize) {
    HttpRequest request;
    std::vector<s_locationConfig> configs;
    size_t i = 100;
    size_t j = 100000;
    size_t k = 999999;
    s_locationConfig config1;
    config1.location = "/";
    config1.loc_max_body_size = i;
    config1.allowed_method = {"POST"};
    configs.push_back(config1);

    s_locationConfig config2;
    config2.location = "/images";
    config2.loc_max_body_size = j;
    config2.allowed_method = {"POST"};
    configs.push_back(config2);

    s_locationConfig config3;
    config3.location = "/dir";
    config3.loc_max_body_size = k;
    config3.allowed_method = {"POST"};
    configs.push_back(config3);

    size_t l = 200;
    request.setContentLength("200");
    request.setBaseLocation("/");
    bool result = HttpRequestFactory::checkMaxBodySize(&request, configs);
    EXPECT_FALSE(result);

    request.setBaseLocation("/images");
    bool result2 = HttpRequestFactory::checkMaxBodySize(&request, configs);
    EXPECT_TRUE(result2);

    request.setBaseLocation("/dir");
    bool result3 = HttpRequestFactory::checkMaxBodySize(&request, configs);
    EXPECT_TRUE(result3);

}
