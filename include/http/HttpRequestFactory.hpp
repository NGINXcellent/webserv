/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactory.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 21:17:02 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/10 08:28:55 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPREQUESTFACTORY_HPP
# define HTTPREQUESTFACTORY_HPP

#include "./HttpRequest.hpp"
#include "./HttpStatus.hpp"
#include "./Server.hpp"

typedef std::map<std::string, std::string> HttpHeaders;
typedef std::vector<s_locationConfig> LocationList;

class HttpRequestFactory {
 public:
  static void           setupHeader(HttpRequest *request, std::string &reqMsg);
  static void           setupRequest(HttpRequest *request, std::string &reqMsg, \
                                     LocationList locations);
  static HttpRequest*   createFrom(std::string &requestMsg, LocationList locs);
  static void         findLocation(HttpRequest *request, LocationList locs);
  static HttpStatusCode check(HttpRequest *request, std::string serverName);
  static bool         checkMaxBodySize(HttpRequest *request, LocationList locs);
  static PostType     setupBodyContentType(HttpRequest *request, HttpHeaders &headers);

 private:
  HttpRequestFactory(void);
  HttpRequestFactory(const HttpRequestFactory& f);
  HttpRequestFactory& operator=(const HttpRequestFactory& t);
  ~HttpRequestFactory(void);


  static std::string  getHeaderValue(std::string headerName, \
                                     HttpHeaders* headers);
};
#endif
