/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:34:36 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/29 18:15:28 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <string>

class HttpRequest {
 public:
  HttpRequest(void);
  ~HttpRequest(void);

  std::string getMethod(void);
  void        setMethod(std::string method);
  std::string getResource(void);
  void        setResource(std::string resource);

 private:
  std::string   resource;
  std::string   method;

  HttpRequest(const HttpRequest& f);
  HttpRequest& operator=(const HttpRequest& t);
};
#endif

