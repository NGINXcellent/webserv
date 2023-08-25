/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypeTests.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 19:56:20 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/24 19:58:22 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "../../../include/http/MimeType.hpp"

TEST(FileTests, BasicMimeTypeTests) {
  std::string text = "filename.txt";
  std::string image = "image.png";
  std::string music = "song.mp3";
  std::string video = "video.mp4";
  std::string pdf = "webserv.pdf";

  EXPECT_EQ("text/plain", MimeType::identify(text));
  EXPECT_EQ("image/png", MimeType::identify(image));
  EXPECT_EQ("audio/mpeg", MimeType::identify(music));
  EXPECT_EQ("video/mp4", MimeType::identify(video));
  EXPECT_EQ("application/pdf", MimeType::identify(pdf));
}

TEST(FileTests, NonExistentMimeTypes) {
  std::string source = "main.c";
  std::string source2 = "main.cpp";
  std::string directory = "/home/";
  std::string app = "hey.exe";

  EXPECT_EQ("application/octet-stream", MimeType::identify(source));
  EXPECT_EQ("application/octet-stream", MimeType::identify(source2));
  EXPECT_EQ("application/octet-stream", MimeType::identify(directory));
  EXPECT_EQ("application/octet-stream", MimeType::identify(app));
}

TEST(FileTests, WeirdNameFormats) {
  std::string multiple_dots2 = "image..png";
  std::string multiple_dots3 = "image...png";
  std::string multiple_dots4 = ".image...png";

  std::string trailing_spaces = "plain.txt ";
  std::string trailing_spaces2 = "plain.txt      ";
  std::string trailing_spaces3 = "plain.txt\t";
  std::string trailing_spaces4 = "plain.txt\t ";
  std::string trailing_spaces5 = "plain.txt\t\t";

  std::string multiple_extensions = "plain.pdf.txt";
  std::string multiple_extensions2 = "plain.txt.pdf";
  std::string multiple_extensions3 = ".txt.hey.pdf";

  std::string empty = "";
  std::string empty1 = "      ";
  std::string empty2 = "    ";

  EXPECT_EQ("image/png", MimeType::identify(multiple_dots2));
  EXPECT_EQ("image/png", MimeType::identify(multiple_dots3));

  EXPECT_EQ("text/plain", MimeType::identify(trailing_spaces));
  EXPECT_EQ("text/plain", MimeType::identify(trailing_spaces2));
  EXPECT_EQ("text/plain", MimeType::identify(trailing_spaces3));
  EXPECT_EQ("text/plain", MimeType::identify(trailing_spaces4));
  EXPECT_EQ("text/plain", MimeType::identify(trailing_spaces5));

  EXPECT_EQ("text/plain", MimeType::identify(multiple_extensions));
  EXPECT_EQ("application/pdf", MimeType::identify(multiple_extensions2));
  EXPECT_EQ("application/pdf", MimeType::identify(multiple_extensions3));

  EXPECT_EQ("", MimeType::identify(empty));
  EXPECT_EQ("", MimeType::identify(empty1));
  EXPECT_EQ("", MimeType::identify(empty2));
}
