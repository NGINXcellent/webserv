/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 20:52:19 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/28 19:45:22 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include <string>
#include <vector>
#include <ctime>
#include <map>

class HttpResponse {
 public:
  HttpResponse(void);
  ~HttpResponse(void);

  std::string   getHeaders(void);
  void          setProtocol(const std::string &protoName, int mainVer, int subVer);
  void          setStatusCode(int responseCode);
  void          setContentType(const std::string &mimeType);
  void          setContentLength(long long fileSize);
  void          setLocation(const std::string& location);
  long long     getContentLength(void);
  void          setMsgBody(char *data);
  char          *getMsgBody(void);
  void          setLastModifiedTime(const std::string &lmfTime);

 private:
  std::string       protocol;
  int               statusCode;
  std::string       statusMessage;
  std::string       serverVersion;
  std::string       modifiedTime;
  std::string       contentType;
  std::string       location;
  long long         contentLength;
  char*             msgBody;

  HttpResponse(const HttpResponse& f);
  HttpResponse& operator=(const HttpResponse& t);
};

#endif
