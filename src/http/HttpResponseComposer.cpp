/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseComposer.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 21:21:24 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/10 21:42:25 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpResponseComposer.hpp"
#include "../../include/http/HttpStatus.hpp"
#include "../../include/http/MimeType.hpp"
#include "../../include/utils/FileReader.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

void HttpResponseComposer::buildErrorResponse(HttpResponse *response, \
                                       int error_code, \
                                       std::map<int, std::string> error_pages, \
                                       int protoMainVersion, \
                                       int protoSubVersion) {
  response->setProtocol("HTTP", protoMainVersion, protoSubVersion);
  response->setStatusCode(error_code);

  if (HttpResponseComposer::getCustomPage(response, error_code, error_pages))
    return;

  std::string content;
  content += "<html><head><title>";
  std::stringstream ss;
  ss << error_code;
  std::string errorMsg = HttpStatus::getMessage(error_code);

  content += ss.str() + " " + errorMsg + "</title></head>";
  content += "<body><center><h1>" + ss.str() + " " + errorMsg + \
    "</h1></center></body>";
  content += "<hr><center>webserv/0.1</center></body></html>";

  std::vector<char> responseContent;
  for (size_t i = 0; i < content.size(); i++) {
    responseContent.push_back(content[i]);
  }

  response->setContentType("text/html");
  response->setMsgBody(responseContent);
  response->setContentLength(responseContent.size());
}

bool HttpResponseComposer::getCustomPage(HttpResponse *response, \
                                         int error_code, \
                                std::map<int, std::string> error_pages) {
  // shim
  std::map<int, std::string>::iterator it = error_pages.find(error_code);
  if (it == error_pages.end())
    return (false);

  std::string filename = error_pages[error_code];
  std::vector<char> resourceData;
  int readStatus = FileReader::getContent(filename, &resourceData);

  if (readStatus != 0) {
    std::cout << "[ERROR]\tCan't find or read custom error page:";
    std::cout << filename << std::endl;
    return (false);
  }

  response->setContentType(MimeType::identify(filename));
  response->setMsgBody(resourceData);
  response->setContentLength(resourceData.size());
  return (true);
}
