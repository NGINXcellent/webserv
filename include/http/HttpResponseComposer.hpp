/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseComposer.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:05:17 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/09 14:09:21 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPRESPONSECOMPOSER_HPP
# define HTTPRESPONSECOMPOSER_HPP

#include <string>
#include <map>

#include "../../include/http/HttpResponse.hpp"

class HttpResponseComposer {
 public:
  static void buildErrorResponse(HttpResponse *response, int error_code, \
                                  std::map<int, std::string> error_pages, \
                                  int protoMainVersion, int protoSubVersion);

 private:
  HttpResponseComposer(void);
  HttpResponseComposer(const HttpResponseComposer& f);
  HttpResponseComposer& operator=(const HttpResponseComposer& t);
  ~HttpResponseComposer(void);

  static bool getCustomPage(HttpResponse *response, int error_code, \
                            std::map<int, std::string> error_pages);
};
#endif
