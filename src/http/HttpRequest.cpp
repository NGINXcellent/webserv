/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:36:19 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/29 18:16:46 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpRequest.hpp"

HttpRequest::HttpRequest(void) {}

HttpRequest::~HttpRequest(void) {}

std::string HttpRequest::getResource(void) {
  return (this->resource);
}

void HttpRequest::setResource(std::string resource) {
  this->resource = resource;
}

std::string HttpRequest::getMethod(void) {
  return (this->method);
}

void HttpRequest::setMethod(std::string method) {
  this->method = method;
}

