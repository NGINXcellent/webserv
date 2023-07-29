/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseFactory.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:05:17 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/28 17:10:45 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPRESPONSEFACTORY_HPP
# define HTTPRESPONSEFACTORY_HPP

#include <string>

#include "../../include/http/HttpResponse.hpp"

class HttpResponseFactory {
 public:
  static HttpResponse   create(std::string resource);

 private:
  HttpResponseFactory(void);
  HttpResponseFactory(const HttpResponseFactory& f);
  HttpResponseFactory& operator=(const HttpResponseFactory& t);
  ~HttpResponseFactory(void);
};
#endif
