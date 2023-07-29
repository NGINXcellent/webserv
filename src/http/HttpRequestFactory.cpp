/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactory.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 21:44:48 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/28 23:01:09 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpRequestFactory.hpp"

HttpRequest *HttpRequestFactory::createFrom(char *requestMsg) {
  std::string msg(requestMsg);
  HttpRequest *request = new HttpRequest();

  // we're just extracting the resource path for now.

  size_t pos = msg.find(' ');
  if (pos != std::string::npos)
    msg.erase(0, pos + 1);
  pos = msg.find(' ');

  std::string resource = "./static_pages" + msg.substr(0, pos);
  request->setResource(resource);
  return (request);
}
