#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "../../../include/http/HttpRequest.hpp"
#include "../../../include/http/HttpRequestFactory.hpp"
#include "../../../include/http/Server.hpp"
#include "../../../include/io/FileReader.hpp"
#include "../../../include/io/FileWriter.hpp"
#include "../../../include/http/HttpResponseComposer.hpp"
#include "../../../include/io/FileSystem.hpp"
#include "../../../src/http/HttpRequestFactory.cpp"
#include "../../../src/http/HttpResponseComposer.cpp"
#include "../../../src/http/HttpParser.cpp"
#include "../../../src/io/FileSystem.cpp"
#include "../../../src/io/FileReader.cpp"
#include "../../../src/io/FileWriter.cpp"

TEST(ServerTest, PostChunked) {
  s_serverConfig DefaultConfig;
  DefaultConfig.srv_max_body_size = 1000;
  HttpRequest request;
  HttpResponse response;
  Server server(DefaultConfig);

  request.setPostType(PostType::Chunked);
  request.setBaseLocation("/test");
  request.setRequestBody("Hello, world!");
  request.setContentLength("10");

  server.post(&request, &response);

  EXPECT_EQ(response.getStatusCode(), HttpStatusCode::No_Content);
}

TEST(ServerTest, PostUrlEncoded) {
  s_serverConfig DefaultConfig;
  DefaultConfig.srv_max_body_size = 1000;
  HttpRequest request;
  HttpResponse response;
  Server server(DefaultConfig);

  request.setPostType(PostType::UrlEncoded);
  request.setBaseLocation("/test");
  request.setRequestBody("name=bar&age=10");
  request.setContentLength("10");

  server.post(&request, &response);

  EXPECT_EQ(response.getStatusCode(), HttpStatusCode::No_Content);
}

TEST(ServerTest, PostMultipart) {
  s_serverConfig DefaultConfig;
  DefaultConfig.srv_max_body_size = 1000;
  HttpRequest request;
  HttpResponse response;
  Server server(DefaultConfig);

  request.setPostType(PostType::Multipart);
  request.setBaseLocation("/test");

  MultiPartMap multiParts;
  multiParts["file"] = "This is a file.";
  multiParts["name"] = "bar";
  multiParts["age"] = "10";

  request.setMultipartMap(multiParts);

  server.post(&request, &response);

  EXPECT_EQ(response.getStatusCode(), HttpStatusCode::Created);
}