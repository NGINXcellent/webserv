/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactory.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 21:17:02 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/09 20:53:54 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPREQUESTFACTORY_HPP
# define HTTPREQUESTFACTORY_HPP

#include "./HttpRequest.hpp"

class HttpRequestFactory {
 public:
  static HttpRequest *createFrom(char *requestMsg, std::string location);
  static int          check(HttpRequest *request);

 private:
  HttpRequestFactory(void);
  HttpRequestFactory(const HttpRequestFactory& f);
  HttpRequestFactory& operator=(const HttpRequestFactory& t);
  ~HttpRequestFactory(void);
};
#endif
