#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <iostream>

#include "../../include/http/HttpRequestFactory.hpp"
#include "../../include/http/HttpRequest.hpp"

void testRequestLine(const char *requestMsg, int expectedStatusCode) {
  HttpRequest *request;
  request = HttpRequestFactory::createFrom(const_cast<char *>(requestMsg));
  EXPECT_EQ(HttpRequestFactory::check(request), expectedStatusCode) << requestMsg;
  delete request;
}

TEST(HttpTests, RequestLineFormatTest)
{
  {
    char requestMsg[] = {
      "GET localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n"};

    HttpRequest *request = HttpRequestFactory::createFrom(requestMsg);
    EXPECT_EQ(HttpRequestFactory::check(request), 0) << request->getProtocolMainVersion();
    delete request;
  }

  {
    char requestMsg[] = {
      "GET localhost:8080/index.html HTTP/1.2\nhost: localhost:8080\n"};

    HttpRequest *request = HttpRequestFactory::createFrom(requestMsg);
    EXPECT_EQ(HttpRequestFactory::check(request), 505) << request->getProtocolMainVersion();
    delete request;
  }
}

TEST(HttpTests, RequestLineMethodTest) {
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

    for (size_t i = 0; i < msgs.size(); i++) {
      testRequestLine(msgs[i].c_str(), 400);
    }
}

TEST(HttpTests, RequestLineProtocolTest) {
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

    for (size_t i = 0; i < msgs.size(); i++) {
      testRequestLine(msgs[i].c_str(), 400);
    }
}

TEST(HttpTests, RequestMissingFieldsTest) {
  std::vector<std::string> msgs;

  msgs.push_back(
    "GET localhost:8080/index.html \nhost: localhost:8080\n");
  msgs.push_back(
    "localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
    "GET HTTP/1.1\nhost: localhost:8080\n");

  for (size_t i = 0; i < msgs.size(); i++) {
    testRequestLine(msgs[i].c_str(), 400);
  }
}

TEST(HttpTests, RequestExtraSpacesTests) {
  std::vector<std::string> msgs;

  msgs.push_back(
    "\tGET localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
    "         GET localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
    "GET            localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
    "GET\t\t\tlocalhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
    "GET\t    localhost:8080/index.html HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
    "GET localhost:8080/index.html          HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
    "GET localhost:8080/index.html\t HTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
    "GET localhost:8080/index.html\t\t\tHTTP/1.1\nhost: localhost:8080\n");
  msgs.push_back(
    "GET localhost:8080/index.html HTTP/1.1       \nhost: localhost:8080\n");
  msgs.push_back(
    "GET localhost:8080/index.html HTTP/1.1\t\nhost: localhost:8080\n");

  for (size_t i = 0; i < msgs.size(); i++) {
    testRequestLine(msgs[i].c_str(), 0);
  }
}

TEST(HttpTests, HttpVersionNotSupportedTest) {
  std::vector<std::string> msgs;

  msgs.push_back(
    "\tGET localhost:8080/index.html HTTP/0.0\nhost: localhost:8080\n");
  msgs.push_back(
    "\tGET localhost:8080/index.html HTTP/1.2\nhost: localhost:8080\n");
  msgs.push_back(
    "\tGET localhost:8080/index.html HTTP/1.3\nhost: localhost:8080\n");
  msgs.push_back(
    "\tGET localhost:8080/index.html HTTP/1.4\nhost: localhost:8080\n");
  msgs.push_back(
    "\tGET localhost:8080/index.html HTTP/2.0\nhost: localhost:8080\n");
  msgs.push_back(
    "\tGET localhost:8080/index.html HTTP/2.1\nhost: localhost:8080\n");
  msgs.push_back(
    "\tGET localhost:8080/index.html HTTP/3.0\nhost: localhost:8080\n");
  msgs.push_back(
    "\tGET localhost:8080/index.html HTTP/12.0\nhost: localhost:8080\n");
  msgs.push_back(
    "\tGET localhost:8080/index.html HTTP/100.0\nhost: localhost:8080\n");

  for (size_t i = 0; i < msgs.size(); i++) {
    testRequestLine(msgs[i].c_str(), 505);
  }
}
