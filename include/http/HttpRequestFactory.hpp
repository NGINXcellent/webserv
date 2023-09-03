/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactory.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 21:17:02 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/03 15:06:27 by lfarias-         ###   ########.fr       */
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
  static HttpRequest*   createFrom(std::string &requestMsg, LocationList locs);

  static HttpStatusCode check(HttpRequest *request);

 private:
  HttpRequestFactory(void);
  HttpRequestFactory(const HttpRequestFactory& f);
  HttpRequestFactory& operator=(const HttpRequestFactory& t);
  ~HttpRequestFactory(void);

  static bool         checkMaxBodySize(HttpRequest *request, LocationList locs);
  static void         findLocation(HttpRequest *request, LocationList locs);
  static std::string  setupBodyContentType(HttpRequest *request, HttpHeaders &headers);
  static std::string  getHeaderValue(std::string headerName, \
                                     HttpHeaders* headers);
};
#endif
