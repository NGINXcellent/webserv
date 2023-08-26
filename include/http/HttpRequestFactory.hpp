/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactory.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 21:17:02 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/25 19:08:51 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPREQUESTFACTORY_HPP
# define HTTPREQUESTFACTORY_HPP

#include "./HttpRequest.hpp"
#include "./Server.hpp"

#include <vector>

/*void P(const std::string& msg, const std::string& str) {
    std::cout << __func__ << "() l. " << __LINE__ << ": " << str << std::endl;
    std::cout << msg << std::endl;
}*/

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
