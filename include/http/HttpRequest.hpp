/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:34:36 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/15 09:24:12 by dvargas          ###   ########.fr       */
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

  std::string   getMethod(void);
  void          setMethod(std::string method);
  std::string   getResource(void);
  void          setResource(std::string resource);
  void          setProtocolName(std::string protocol);
  std::string   getProtocolName(void);
  void          setProtocolVersion(int main, int sub);
  int           getProtocolMainVersion(void);
  int           getProtocolSubVersion(void);
  std::string   getHost(void);
  void          setHost(std::string nHost);
  std::string   getModifiedTimestampCheck(void);
  void          setModifiedTimestampCheck(std::string timestamp);
  std::string   getUnmodifiedSinceTimestamp(void);
  void          setUnmodifiedSinceTimestamp(std::string timestamp);
  std::vector<std::string>   getAllowedMethods(void);
  void          setAllowedMethods(std::vector<std::string> allowedList);
  void setResponseStatusCode(int toset);
  int getResponseStatusCode();

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

