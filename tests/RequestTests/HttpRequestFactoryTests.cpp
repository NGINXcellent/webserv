/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactoryTests.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 20:01:35 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/14 15:57:51 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

#include "../../include/http/HttpRequest.hpp"
#include "../../include/http/HttpRequestFactory.hpp"
#include "../../include/http/Server.hpp"


std::string createLocation(char *buffer, std::vector<s_locationConfig> locations, HttpRequest *request);

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

    char buffer1[] = "GET / HTTP/1.1";
    char buffer2[] = "GET /images HTTP/1.1";
    char buffer3[] = "GET /teste4/vamos/testar/aqui HTTP/1.1";
    char buffer4[] = "GET /dir HTTP/1.1";
    HttpRequest request;

    std::string result1 = createLocation(buffer1, configs, &request);
    EXPECT_EQ(result1, "./index.html");

    std::string result2 = createLocation(buffer2, configs, &request);
    EXPECT_EQ(result2, "./var/www/images");

    std::string result3 = createLocation(buffer3, configs, &request);
    EXPECT_EQ(result3, "./teste4/vamos/testar/aqui");

    std::string result4 = createLocation(buffer4, configs, &request);
    EXPECT_EQ(result4, "../../InputTests/test_files/fail/nestedServer.conf");
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
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host: localhost:8080\n";
    request += "Connection: keep-alive\n";
    request += "Accept-Language: en-US,en\n";

    std::vector<s_locationConfig> locations;
    s_locationConfig locationOne;
    locationOne.location = "/index.html";
    locations.push_back(locationOne);
    testRequestLine(request.c_str(), 0, locations);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "   host: localhost:8080\n";
    request += "Connection: keep-alive\n";
    request += "Accept-Language: en-US,en\n";

    std::vector<s_locationConfig> locations;
    s_locationConfig locationOne;
    locationOne.location = "/index.html";
    locations.push_back(locationOne);
    testRequestLine(request.c_str(), 400, locations);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host: localhost:8080\n";
    request += "\tConnection: keep-alive\n";
    request += "Accept-Language: en-US,en\n";

    std::vector<s_locationConfig> locations;
    s_locationConfig locationOne;
    locationOne.location = "/index.html";
    locations.push_back(locationOne);
    testRequestLine(request.c_str(), 400, locations);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host: localhost:8080\n";
    request += "Connection: keep-alive\n";
    request += "   \tAccept-Language: en-US,en\n";

    std::vector<s_locationConfig> locations;
    s_locationConfig locationOne;
    locationOne.location = "/index.html";
    locations.push_back(locationOne);
    testRequestLine(request.c_str(), 400, locations);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host:       localhost:8080\n";

    std::vector<s_locationConfig> locations;
    s_locationConfig locationOne;
    locationOne.location = "/index.html";
    locations.push_back(locationOne);
    testRequestLine(request.c_str(), 0, locations);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host:\t\tlocalhost:8080\n";

    std::vector<s_locationConfig> locations;
    s_locationConfig locationOne;
    locationOne.location = "/index.html";
    locations.push_back(locationOne);
    testRequestLine(request.c_str(), 0, locations);
  }
}
