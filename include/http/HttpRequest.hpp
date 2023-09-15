/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:34:36 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/15 10:41:37 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <string>
#include <vector>
#include <map>

enum PostType {
  None,
  Multipart,
  Chunked,
  UrlEncoded
};

typedef std::map<std::string, std::string> MultiPartMap;

class HttpRequest {
 public:
  HttpRequest(void);
  ~HttpRequest(void);

  bool                  isDirListActive(void);
  void                  setDirListActive(const bool &active);
  const std::string&    getMethod(void);
  void                  setMethod(const std::string &method);
  const std::string&    getResource(void);
  void                  setResource(const std::string &resource);
  void                 setProtocolName(const std::string &protocol);
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
  void                  setupContentType(const std::string &msg, HttpRequest *request);
  PostType              getPostType(void);
  void                  setPostType(PostType type);
  const std::string&    getRequestBody(void);
  void                  setRequestBody(const std::string &body);
  const std::string&    getRoot(void);
  void                  setRoot(const std::string &body);
  size_t                getContentLength(void);
  void                  setContentLength(const std::string &sizeStr);
  const std::string&    getIndexPath(void);
  void                  setIndexPath(const std::string &iPath);
  const std::string&    getRedirectionPath(void);
  void                  setRedirectionPath(const std::string &loc);
  int                   getRedirectionCode(void);
  void                  setRedirectionCode(int toset);
  const std::string&    getLocationWithoutIndex(void);
  void                  setLocationWithoutIndex(const std::string &loc);
  void                  setBoundary(const std::string &boundary);
  const std::string&    getBoundary(void);
  void                  setMultipartMap(const MultiPartMap& parts);
  const MultiPartMap&   getMultipartMap(void);
  void                  setBaseLocation(std::string baseLocation);
  std::string           getBaseLocation(void);
  bool                  isHeaderReady(void);
  void                  setHeaderReady(bool opt);
  bool                  isRequestReady(void);
  void                  setRequestReady(bool set);
  void                  setCGI(bool toset, std::string cgiPath, std::string cgiExtension);
  bool                  getCGI(void);
  std::string           getCGIPath(void);
  std::string           getCGIExtension(void);
  void                  setQueryString(std::string query);
  std::string           getQueryString(void);
  /*bool                  isBodyReady(void);
  void                  setBodyReady(bool opt);*/


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
  std::string               indexPath;
  std::string               redirectionPath;
  std::string               baseLocation;
  std::string               location;
  std::string               locationWithoutIndex;
  std::string               boundary;
  MultiPartMap              multipartMap;
  PostType                  postType;
  std::string               root;
  bool                      hasHeader;
  bool                      hasBodyt;
  bool                      requestReady;
  bool                      isCGI;
  std::string               cgiPath;
  std::string               cgiExtension;
  std::string               queryString;

  HttpRequest(const HttpRequest& f);
  HttpRequest& operator=(const HttpRequest& t);
};
#endif

