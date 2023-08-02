
#include <gtest/gtest.h>
#include "../../include/http/HttpResponse.hpp"
#include <string>

TEST(HttpTests, ServerHeaderTests)
{
  std::string expected = "Server: webserv/0.1";
  HttpResponse response;
  std::string headers = response.getHeaders();

  int pos = headers.find('\n', 0);
  std::string serverLine = headers.substr(++pos, expected.size());
  EXPECT_TRUE(serverLine == expected);
}
