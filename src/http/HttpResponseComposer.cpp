/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseComposer.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 21:21:24 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/05 21:27:06 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpResponseComposer.hpp"
#include "../../include/http/HttpStatus.hpp"

#include <sstream>

void HttpResponseComposer::buildErrorResponse(HttpResponse *response, \
                                              int error_code, \
                                              int protoMainVersion, \
                                              int protoSubVersion) {
  response->setStatusCode(error_code);
  response->setContentType("text/html");

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

  response->setProtocol("HTTP", protoMainVersion, protoSubVersion);
  response->setMsgBody(responseContent);
  response->setContentLength(responseContent.size());
}
