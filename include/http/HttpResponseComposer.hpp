/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseComposer.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:05:17 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/27 20:22:42 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPRESPONSECOMPOSER_HPP
# define HTTPRESPONSECOMPOSER_HPP

#include <dirent.h>
#include <string>
#include <map>

#include "../../include/http/HttpResponse.hpp"
#include "../../include/http/HttpRequest.hpp"

class HttpResponseComposer {
 public:
  static void buildResponse(HttpRequest *req, HttpResponse *res, \
                            char *resourceData, \
                            long long resourceSize);

  static void buildDirListResponse(HttpRequest *request, HttpResponse *response, \
                                   std::map<std::string, struct dirent*> &entries);

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

  static void formatDirListStyle(const std::string &pathName, std::string &site);

  static void formatEntryName(const std::string &name, std::string &responseStr, \
                              std::map<std::string, struct dirent *>::iterator it);
};
#endif
