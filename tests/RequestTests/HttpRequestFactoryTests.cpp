/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactoryTests.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 20:01:35 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/05 20:53:50 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

  for (size_t i = 0; i < reqLines.size(); i++) {
    testRequestLine(reqLines[i].c_str(), 0);
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

    for (size_t i = 0; i < msgs.size(); i++) {
      testRequestLine(msgs[i].c_str(), 400);
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

    for (size_t i = 0; i < msgs.size(); i++) {
      testRequestLine(msgs[i].c_str(), 400);
    }
}

TEST(RequestTests, RequestMissingFieldsTest) {
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

TEST(RequestTests, RequestExtraSpacesTests) {
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

  for (size_t i = 0; i < msgs.size(); i++) {
    testRequestLine(msgs[i].c_str(), 505);
  }
}

TEST(RequestTests, HttpVersionOnePointZeroTest) {

  std::vector<std::string> msgs;

  msgs.push_back(
    "GET localhost:8080/index.html HTTP/1.0\nhost: localhost:8080\n");
  msgs.push_back(
    "GET localhost:8080/index.html HTTP/1.0\nhost:\n");
  msgs.push_back(
    "GET localhost:8080/index.html HTTP/1.0\n");

  for (size_t i = 0; i < msgs.size(); i++) {
    testRequestLine(msgs[i].c_str(), 0);
  }
}

TEST(RequestTests, HttpRequestAttributesFormat) {
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host: localhost:8080\n";
    request += "Connection: keep-alive\n";
    request += "Accept-Language: en-US,en\n";

    testRequestLine(request.c_str(), 0);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "   host: localhost:8080\n";
    request += "Connection: keep-alive\n";
    request += "Accept-Language: en-US,en\n";

    testRequestLine(request.c_str(), 400);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host: localhost:8080\n";
    request += "\tConnection: keep-alive\n";
    request += "Accept-Language: en-US,en\n";

    testRequestLine(request.c_str(), 400);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host: localhost:8080\n";
    request += "Connection: keep-alive\n";
    request += "   \tAccept-Language: en-US,en\n";

    testRequestLine(request.c_str(), 400);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host:       localhost:8080\n";

    testRequestLine(request.c_str(), 0);
  }
  {
    std::string request;
    request += "GET localhost:8080/index.html HTTP/1.1\n";
    request += "host:\t\tlocalhost:8080\n";

    testRequestLine(request.c_str(), 0);
  }
}
