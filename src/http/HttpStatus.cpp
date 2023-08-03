/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 16:15:50 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/03 17:47:50 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpStatus.hpp"

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
  messages.insert(std::make_pair(204, "No Content"));

  // 400 class - client errors
  messages.insert(std::make_pair(400, "Bad Request"));
  messages.insert(std::make_pair(403, "Forbidden"));
  messages.insert(std::make_pair(404, "Not Found"));
  messages.insert(std::make_pair(405, "Method Not Allowed"));
  messages.insert(std::make_pair(411, "Length Required"));
  messages.insert(std::make_pair(413, "Payload Too Large"));
  messages.insert(std::make_pair(414, "URI Too Long"));
  messages.insert(std::make_pair(415, "Unsupported Media Type"));

  // 500 class - server errors
  messages.insert(std::make_pair(501, "Not Implemented"));

  // todo: Add Other classes of msgs
}
