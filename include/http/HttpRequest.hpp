/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:34:36 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/03 20:34:44 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <string>

class HttpRequest {
 public:
  HttpRequest(void);
  ~HttpRequest(void);

  std::string   getMethod(void);
  void          setMethod(std::string method);
  std::string   getResource(void);
  void          setResource(std::string resource);
  void          setProtocolName(std::string protocol);
  std::string   getProtocolName(void);
  void          setProtocolVersion(int main, int sub);
  int           getProtocolMainVersion(void);
  int           getProtocolSubVersion(void);
  void          setHost(std::string nHost);
  std::string   getHost(void);

 private:
  std::string   protocolName;
  int           protocolMainVersion;
  int           protocolSubVersion;
  std::string   host;
  std::string   resource;
  std::string   method;

  HttpRequest(const HttpRequest& f);
  HttpRequest& operator=(const HttpRequest& t);
};
#endif

