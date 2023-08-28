/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientTests.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 13:52:13 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/28 09:36:56 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <gtest/gtest.h>
#include "../../../include/io/Client.hpp"
#include <string>

TEST(ClientTests, ClientStringReference)
{
  Client newClient(-1, NULL, 0, time(0));
  std::string &buffer = newClient.getBuffer();

  EXPECT_EQ(buffer, "");
  EXPECT_EQ(buffer.size(), 0);

  std::string testStr = "This is a test!";
  buffer += testStr; 
  EXPECT_EQ(testStr, newClient.getBuffer());

  newClient.reset(); // clean internal buffer
  EXPECT_NE(testStr, newClient.getBuffer());
  EXPECT_EQ(newClient.getBuffer().size(), 0);
}

TEST(ClientTests, ClientTimeoutTests)
{
  int timeoutLength = 60;
  size_t currentTime = time(0);
  Client newClient(-1, NULL, 0, currentTime);
  EXPECT_EQ(newClient.getTimeout(), currentTime + timeoutLength);

  currentTime = 0;
  Client newClient2(-1, NULL, 0, currentTime);
  EXPECT_EQ(newClient2.getTimeout(), currentTime + timeoutLength);

  // Add a test case for negative time.
}