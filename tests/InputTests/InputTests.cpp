#include <gtest/gtest.h>

bool  check_args(int argc, const char **argv);

TEST(InputTests, BasicTests)
{
  {
    const char *argv[2] = {"webserv", "myserver.conf"};
    EXPECT_TRUE(check_args(2, argv));
  }

  {
    const char *argv[2] = {"webserv", "a.conf"};
    EXPECT_TRUE(check_args(2, argv));
  }

  {
    const char *argv[2] = {"webserv", ".conf"};
    EXPECT_FALSE(check_args(2, argv));
  }
}

TEST(InputTests, ExtensionTests)
{
  {
    const char *argv[2] = {"webserv", "myserver.con"};
    EXPECT_FALSE(check_args(2, argv));
  }

  {
    const char *argv[2] = {"webserv", "myserver.config"};
    EXPECT_FALSE(check_args(2, argv));
  }

  {
    const char *argv[2] = {"webserv", "myserverconfig"};
    EXPECT_FALSE(check_args(2, argv));
  }

  {
    const char *argv[2] = {"webserv", "myserver.cnf"};
    EXPECT_FALSE(check_args(2, argv));
  }

  {
    const char *argv[2] = {"webserv", "config"};
    EXPECT_FALSE(check_args(2, argv));
  }

  {
    const char *argv[2] = {"webserv", ".cnf"};
    EXPECT_FALSE(check_args(2, argv));
  }
}
