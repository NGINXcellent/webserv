/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 20:52:19 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/28 20:01:00 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include <string>
#include <vector>

class HttpResponse {
 public:
  HttpResponse(void);
  ~HttpResponse(void);

  std::string   getHeaders(void);
  void          setStatusCode(int responseCode);
  void          setContentType(std::string mimeType);
  void          setMsgBody(const std::vector<char>& data);

 private:
  int               statusCode;
  std::string       statusMessage;
  std::string       contentType;
  int               contentLength;
  int               dataSize;
  std::vector<char> msgBody;

  HttpResponse(const HttpResponse& f);
  HttpResponse& operator=(const HttpResponse& t);
};

#endif
