/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseComposer.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:05:17 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/05 21:26:26 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPRESPONSECOMPOSER_HPP
# define HTTPRESPONSECOMPOSER_HPP

#include <string>

#include "../../include/http/HttpResponse.hpp"

class HttpResponseComposer {
 public:
  static void buildErrorResponse(HttpResponse *response, int error_code, \
                                  int protoMainVersion, int protoSubVersion);

 private:
  HttpResponseComposer(void);
  HttpResponseComposer(const HttpResponseComposer& f);
  HttpResponseComposer& operator=(const HttpResponseComposer& t);
  ~HttpResponseComposer(void);
};
#endif
