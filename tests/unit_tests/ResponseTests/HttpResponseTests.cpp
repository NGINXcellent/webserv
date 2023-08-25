
#include <cstdlib>
#include <gtest/gtest.h>
#include "../../../include/http/HttpResponse.hpp"
#include <string>

TEST(ResponseTests, ServerHeaderTests)
{
  std::string expected = "Server: webserv/0.1";
  HttpResponse response;
  std::string headers = response.getHeaders();

  int pos = headers.find('\n', 0);
  std::string serverLine = headers.substr(++pos, expected.size());
  EXPECT_TRUE(serverLine == expected);
}

TEST(ResponseTests, ContentLengthTest) {
  // insert a response status that doesn't have a body like 304
  { 
    HttpResponse response;
    response.setProtocol("HTTP", 1, 1);
    response.setStatusCode(304);
    std::string responseStr = response.getHeaders();
    EXPECT_NE(responseStr.find("Not Modified"), std::string::npos);
    EXPECT_EQ(responseStr.find("Content-Length"), std::string::npos);
  }
  
  {
    HttpResponse response;
    response.setProtocol("HTTP", 1, 1);
    response.setStatusCode(304);
    response.setContentLength(42);
    std::string responseStr = response.getHeaders();
    EXPECT_NE(responseStr.find("Not Modified"), std::string::npos);
    EXPECT_EQ(responseStr.find("Content-length"), std::string::npos);
  }

  {
    HttpResponse response;
    response.setProtocol("HTTP", 1, 1);
    response.setStatusCode(200);  
    char *msg = new char[] {"This is an awesome little message"};
    response.setMsgBody(msg);
    response.setContentLength(strlen(msg));
    std::string responseStr = response.getHeaders();
    EXPECT_NE(responseStr.find("OK"), std::string::npos);
    size_t headerPos = responseStr.find("Content-length");
    ASSERT_NE(headerPos, std::string::npos);
    size_t valuePos = responseStr.find_first_of("0123456789", headerPos); 
    int fileSize = strtol(responseStr.substr(valuePos).data(), NULL, 10);
    int msgSize = strlen(msg);
    EXPECT_EQ(fileSize, msgSize);
  }
  
}
