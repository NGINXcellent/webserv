/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactory.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 21:17:02 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/02 19:40:49 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPREQUESTFACTORY_HPP
# define HTTPREQUESTFACTORY_HPP

#include "./HttpRequest.hpp"
#include "./HttpStatus.hpp"
#include "./Server.hpp"

#include <vector>

class HttpRequestFactory {
 public:
  static void           findLocation(const std::string &buffer, \
                                     std::vector<s_locationConfig> locations, \
                                     HttpRequest *request);

  static HttpRequest*   createFrom(std::string &requestMsg, \
                                 std::vector<s_locationConfig> locations);

  static HttpStatusCode check(HttpRequest *request);

  static bool           checkMaxBodySize(HttpRequest *request, \
                                         std::vector<s_locationConfig> locs);

 private:
  HttpRequestFactory(void);
  HttpRequestFactory(const HttpRequestFactory& f);
  HttpRequestFactory& operator=(const HttpRequestFactory& t);
  ~HttpRequestFactory(void);
};
#endif
