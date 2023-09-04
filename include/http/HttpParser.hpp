/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 20:31:42 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/03 19:00:08 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

#include "../../include/http/HttpRequest.hpp"

#include <string>
#include <vector>
#include <map>

typedef std::map<std::string, std::string> HttpHeaders;

class HttpParser {
 public:
  void parseRequestLine(std::string *requestLine, HttpRequest *request);
  bool parseProtocolVersion(const std::string &input, int *mainVer, int *subVer);
  bool parseHeaders(std::string *msg, HttpHeaders *headers);
};
#endif
