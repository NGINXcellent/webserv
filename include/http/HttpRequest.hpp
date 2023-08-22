/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:34:36 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/22 19:03:22 by lfarias-         ###   ########.fr       */
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

  const std::string&    getMethod(void);
  void                  setMethod(const std::string &method);
  const std::string&    getResource(void);
  void                  setResource(const std::string &resource);
  void                  setProtocolName(const std::string &protocol);
  const std::string&    getProtocolName(void);
  void                  setProtocolVersion(int main, int sub);
  int                   getProtocolMainVersion(void);
  int                   getProtocolSubVersion(void);
  const std::string&    getHost(void);
  void                  setHost(const std::string &nHost);
  const std::string&    getModifiedTimestampCheck(void);
  void                  setModifiedTimestampCheck(const std::string &timestamp);
  const std::string&    getUnmodifiedSinceTimestamp(void);
  void                  setUnmodifiedSinceTimestamp(const std::string &timestamp);
  const std::vector<std::string>   &getAllowedMethods(void);
  void                  setAllowedMethods(const std::vector<std::string> &allowedList);
  void                  setResponseStatusCode(int toset);
  int                   getResponseStatusCode(void);
  void                  setupContentType(const std::string &msg, HttpRequest *request);
  const std::string&    getPostType(void);
  void                  setPostType(const std::string &type);
  const std::string&    getRequestBody(void);
  void                  setRequestBody(const std::string &body);
  size_t                getContentLength(void);
  void                  setContentLength(const std::string &sizeStr);
  const std::string&    getLocationTest(void);
  void                  setLocationTest(const std::string &loc);

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
  size_t                    bodySize;
  std::string               requestBody;
  std::string               locationTest;

  //TESTANDO COM O METODO POST
  std::string               postType;

  HttpRequest(const HttpRequest& f);
  HttpRequest& operator=(const HttpRequest& t);
};
#endif

