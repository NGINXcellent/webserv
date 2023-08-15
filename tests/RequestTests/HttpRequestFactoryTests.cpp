/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactoryTests.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 20:01:35 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/14 22:22:50 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

#include "../../include/http/HttpRequest.hpp"
#include "../../include/http/HttpRequestFactory.hpp"
#include "../../include/http/Server.hpp"

void testRequestLine(const char *requestMsg, int expectedStatusCode,
                     std::vector<s_locationConfig> locations) {
  HttpRequest *request;
  request = HttpRequestFactory::createFrom(const_cast<char *>(requestMsg), locations);
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
  locationOne.location = "/index.html";
  locations.push_back(locationOne);

  for (size_t i = 0; i < reqLines.size(); i++) {
    testRequestLine(reqLines[i].c_str(), 0, locations);
  }
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
}
