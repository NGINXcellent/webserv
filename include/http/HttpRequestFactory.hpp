/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactory.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 21:17:02 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/25 10:21:54 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPREQUESTFACTORY_HPP
# define HTTPREQUESTFACTORY_HPP

#include "./HttpRequest.hpp"
#include "./Server.hpp"

#include <vector>

#define P(msg) \
    std::cout << __func__ << "() l. " << __LINE__ << ": " << (msg) << '\n' \
              << msg << std::endl

class HttpRequestFactory {
 public:
  static void createLocation(const std::string &buffer, std::vector<s_locationConfig> locations, HttpRequest *request);
  static HttpRequest *createFrom(std::string &requestMsg, std::vector<s_locationConfig> locations);
  static int          check(HttpRequest *request);

 private:
  HttpRequestFactory(void);
  HttpRequestFactory(const HttpRequestFactory& f);
  HttpRequestFactory& operator=(const HttpRequestFactory& t);
  ~HttpRequestFactory(void);
};
#endif
