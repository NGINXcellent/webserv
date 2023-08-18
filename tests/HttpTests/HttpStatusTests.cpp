#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "../../include/http/HttpStatus.hpp"

TEST(HttpTests, BasicStatusTests) {
  int cont = 100;
  int ok = 200;
  int not_found = 404;
  int forbidden = 403;
  int server_error = 500;
  int not_supported = 505;

  EXPECT_EQ("Continue", HttpStatus::getMessage(cont));
  EXPECT_EQ("OK", HttpStatus::getMessage(ok));
  EXPECT_EQ("Not Found", HttpStatus::getMessage(not_found));
  EXPECT_EQ("Forbidden", HttpStatus::getMessage(forbidden));
  EXPECT_EQ("Internal Server Error", HttpStatus::getMessage(server_error));
  EXPECT_EQ("HTTP Version Not Supported", HttpStatus::getMessage(not_supported));
}

TEST(HttpTests, NonExistentStatus) {
  EXPECT_EQ("", HttpStatus::getMessage(0));
  EXPECT_EQ("", HttpStatus::getMessage(-1));
  EXPECT_EQ("", HttpStatus::getMessage(42));
  EXPECT_EQ("", HttpStatus::getMessage(101));
  EXPECT_EQ("", HttpStatus::getMessage(600));
}
