/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:34:36 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/18 17:48:19 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <string>
#include <vector>

class HttpRequest {
 public:
  HttpRequest(void);
  ~HttpRequest(void);

  const std::string     &getMethod(void);
  void                  setMethod(const std::string &method);
  const std::string     &getResource(void);
  void                  setResource(const std::string &resource);
  void                  setProtocolName(const std::string &protocol);
  const std::string     &getProtocolName(void);
  void                  setProtocolVersion(int main, int sub);
  int                   getProtocolMainVersion(void);
  int                   getProtocolSubVersion(void);
  const std::string     &getHost(void);
  void                  setHost(const std::string &nHost);
  const std::string     &getModifiedTimestampCheck(void);
  void                  setModifiedTimestampCheck(const std::string &timestamp);
  const std::string     &getUnmodifiedSinceTimestamp(void);
  void                  setUnmodifiedSinceTimestamp(const std::string &timestamp);
  const std::vector<std::string>   &getAllowedMethods(void);
  void                  setAllowedMethods(const std::vector<std::string> &allowedList);
  void                  setResponseStatusCode(int toset);
  int                   getResponseStatusCode(void);

 private:
  std::string               protocolName;
  int                       protocolMainVersion;
  int                       protocolSubVersion;
  std::string               host;
  std::string               resource;
  std::string               method;
  std::string               modifiedTimestampCheck;
  std::string               unmodifiedSinceTimestamp;
  int                       responseStatusCode;
  std::vector<std::string>  allowedMethodList;

  HttpRequest(const HttpRequest& f);
  HttpRequest& operator=(const HttpRequest& t);
};
#endif

