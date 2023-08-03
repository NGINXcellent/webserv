#include <gtest/gtest.h>
#include "../../include/input/InputHandler.hpp"
#include <fstream>

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

TEST(InputTests, ArgcTests)
{
    const char *argv[2] = {"webserv", "default.conf"};

    EXPECT_FALSE(InputHandler::check_args(42, argv));
    EXPECT_FALSE(InputHandler::check_args(3, argv));
    EXPECT_FALSE(InputHandler::check_args(0, argv));
    EXPECT_FALSE(InputHandler::check_args(-2, argv));
    EXPECT_FALSE(InputHandler::check_args(-42, argv));
    EXPECT_FALSE(InputHandler::check_args(-3, argv));
}

TEST(CheckConfTests, BasicTest)
{
  {
    const char *configFile = "/home/dvargas/Desktop/webserv/tests/InputTests/test_files/default.conf";
    char *configFileMutable = const_cast<char*>(configFile);
    EXPECT_NO_THROW(InputHandler input(configFileMutable));
  }
  {
    const char *configFile = "/home/dvargas/Desktop/webserv/tests/InputTests/test_files/failDefault.conf";
    char *configFileMutable = const_cast<char*>(configFile);
    EXPECT_THROW(InputHandler input(configFileMutable), std::runtime_error);
  }
}

TEST(CheckConfTests, MultipleServersTest)
{
  {
    const char *configFile = "/home/dvargas/Desktop/webserv/tests/InputTests/test_files/default.conf";
    char *configFileMutable = const_cast<char*>(configFile);
    InputHandler input(configFileMutable);
    EXPECT_EQ(input.serverVector->size(), 1);
  }
  {
    const char *configFile = "/home/dvargas/Desktop/webserv/tests/InputTests/test_files/defaultMultiple.conf";
    char *configFileMutable = const_cast<char*>(configFile);
    InputHandler input(configFileMutable);
    EXPECT_EQ(input.serverVector->size(), 6);
  }
}

TEST(CheckConfTests, FailTests)
{
  {
    const char *configFile = "/home/dvargas/Desktop/webserv/tests/InputTests/test_files/fail/doubleEntry.conf";
    char *configFileMutable = const_cast<char*>(configFile);
    EXPECT_THROW(InputHandler input(configFileMutable), std::runtime_error);
  }
  {
    const char *configFile = "/home/dvargas/Desktop/webserv/tests/InputTests/test_files/fail/emptyEntry.conf";
    char *configFileMutable = const_cast<char*>(configFile);
    EXPECT_THROW(InputHandler input(configFileMutable), std::runtime_error);
  }
  {
    const char *configFile = "/home/dvargas/Desktop/webserv/tests/InputTests/test_files/fail/invalidListen.conf";
    char *configFileMutable = const_cast<char*>(configFile);
    EXPECT_THROW(InputHandler input(configFileMutable), std::runtime_error);
  }
  {
    const char *configFile = "/home/dvargas/Desktop/webserv/tests/InputTests/test_files/fail/invalidServerName.conf";
    char *configFileMutable = const_cast<char*>(configFile);
    EXPECT_THROW(InputHandler input(configFileMutable), std::runtime_error);
  }
  {
    const char *configFile = "/home/dvargas/Desktop/webserv/tests/InputTests/test_files/fail/nestedServer.conf";
    char *configFileMutable = const_cast<char*>(configFile);
    EXPECT_THROW(InputHandler input(configFileMutable), std::runtime_error);
  }
  {
    const char *configFile = "/home/dvargas/Desktop/webserv/tests/InputTests/test_files/fail/noDot.conf";
    char *configFileMutable = const_cast<char*>(configFile);
    EXPECT_THROW(InputHandler input(configFileMutable), std::runtime_error);
  }
  {
    const char *configFile = "/home/dvargas/Desktop/webserv/tests/InputTests/test_files/fail/unknowEntry.conf";
    char *configFileMutable = const_cast<char*>(configFile);
    EXPECT_THROW(InputHandler input(configFileMutable), std::runtime_error);
  }
}