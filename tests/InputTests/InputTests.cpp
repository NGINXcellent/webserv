#include <gtest/gtest.h>
#include "../../include/input/InputHandler.hpp"

TEST(InputTests, BasicTests)
{
  {
    const char *argv[2] = {"webserv", "myserver.conf"};
    EXPECT_TRUE(InputHandler::check_args(2, argv));
  }

  {
    const char *argv[2] = {"webserv", "a.conf"};
    EXPECT_TRUE(InputHandler::check_args(2, argv));
  }

  {
    const char *argv[2] = {"webserv", ".conf"};
    EXPECT_FALSE(InputHandler::check_args(2, argv));
  }

  {
    const char *argv[2] = {NULL, NULL};
    EXPECT_FALSE(InputHandler::check_args(2, argv));
  }
}

TEST(InputTests, ExtensionTests)
{
  {
    const char *argv[2] = {"webserv", "myserver.con"};
    EXPECT_FALSE(InputHandler::check_args(2, argv));
  }

  {
    const char *argv[2] = {"webserv", "myserver.config"};
    EXPECT_FALSE(InputHandler::check_args(2, argv));
  }

  {
    const char *argv[2] = {"webserv", "myserverconfig"};
    EXPECT_FALSE(InputHandler::check_args(2, argv));
  }

  {
    const char *argv[2] = {"webserv", "myserver.cnf"};
    EXPECT_FALSE(InputHandler::check_args(2, argv));
  }

  {
    const char *argv[2] = {"webserv", "config"};
    EXPECT_FALSE(InputHandler::check_args(2, argv));
  }

  {
    const char *argv[2] = {"webserv", ".cnf"};
    EXPECT_FALSE(InputHandler::check_args(2, argv));
  }
}
