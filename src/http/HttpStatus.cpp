/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 16:15:50 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/23 23:10:10 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpStatus.hpp"
#include <utility>

std::map<int, std::string> HttpStatus::messages;
bool HttpStatus::is_init = false;

std::string HttpStatus::getMessage(int statusCode) {
  if (!is_init) {
    HttpStatus::init();
    is_init = true;
  }
  return messages[statusCode];
}

void HttpStatus::init(void) {
  messages.insert(std::make_pair(100, "Continue"));
  // 200 class
  messages.insert(std::make_pair(200, "OK"));
  messages.insert(std::make_pair(201, "Created"));
  messages.insert(std::make_pair(204, "No Content"));

  // 300 class - redirection
  messages.insert(std::make_pair(301, "Moved Permanently"));
  messages.insert(std::make_pair(304, "Not Modified"));
  messages.insert(std::make_pair(308, "Permanent Redirect"));
  messages.insert(std::make_pair(302, "Found"));
  messages.insert(std::make_pair(303, "See Other"));
  messages.insert(std::make_pair(307, "Temporary Redirect"));

  // 400 class - client errors
  messages.insert(std::make_pair(400, "Bad Request"));
  messages.insert(std::make_pair(403, "Forbidden"));
  messages.insert(std::make_pair(404, "Not Found"));
  messages.insert(std::make_pair(405, "Method Not Allowed"));
  messages.insert(std::make_pair(411, "Length Required"));
  messages.insert(std::make_pair(412, "Precondition Failed"));
  messages.insert(std::make_pair(413, "Payload Too Large"));
  messages.insert(std::make_pair(414, "URI Too Long"));
  messages.insert(std::make_pair(415, "Unsupported Media Type"));

  // 500 class - server errors
  messages.insert(std::make_pair(500, "Internal Server Error"));
  messages.insert(std::make_pair(501, "Not Implemented"));
  messages.insert(std::make_pair(505, "HTTP Version Not Supported"));

  // todo: Add Other classes of msgs
}
