/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:34:36 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/25 18:26:17 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <string>
#include <vector>

struct s_multipartStruct {
  std::string                 name;
  std::string                 content;
};

class HttpRequest {
 public:
  HttpRequest(void);
  ~HttpRequest(void);

  bool                  isDirListActive(void);
  void                  setDirListActive(const std::string &active);
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
 void                  setupContentType(const std::string &msg, HttpRequest *request);
  const std::string&    getPostType(void);
  void                  setPostType(const std::string &type);
  const std::string&    getRequestBody(void);
  void                  setRequestBody(const std::string &body);
  size_t                getContentLength(void);
  void                  setContentLength(const std::string &sizeStr);
  const std::string&    getLocation(void);
  void                  setLocation(const std::string &loc);
  const std::string&    getLocationWithoutIndex(void);
  void                  setLocationWithoutIndex(const std::string &loc);
  void                  setBoundary(const std::string &boundary);
  const std::string&    getBoundary(void);
  void                  setMultipartStruct(const std::vector<s_multipartStruct>& parts);
  const std::vector<s_multipartStruct>& getMultipartStruct();

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
  bool                      autoindex;
  std::string               location;
  std::string               locationWithoutIndex;
  std::string               boundary;
  std::vector<s_multipartStruct>  multipartStructVector;
  std::string               postType;

  HttpRequest(const HttpRequest& f);
  HttpRequest& operator=(const HttpRequest& t);
};
#endif

