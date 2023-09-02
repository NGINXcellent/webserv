/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 16:13:54 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/02 15:07:44 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPSTATUS_HPP
# define HTTPSTATUS_HPP

#include <string>
#include <map>

enum status_code {
  Ok = 200,
  Created = 201,
  No_Content = 204,
  Moved_Permanently = 301,
  Found = 302,
  See_Other = 303,
  Not_Modified = 304,
  Temporary_Redirect = 307,
  Permanent_Redirect = 308,
  Bad_Request = 400,
  Forbidden = 403,
  Not_Found = 404,
  Method_Not_Allowed = 405,
  Length_Required = 411,
  Precondition_Failed = 412,
  Payload_Too_Large = 413,
  Uri_Too_Long = 414,
  Unsupported_Media_Type = 415,
  Internal_Server_Error = 500,
  Not_Implemented = 501,
  Http_Ver_Unsupported = 505
};

typedef status_code HttpStatusCode;

class HttpStatus {
 public:
  static const std::string& getMessage(int statusCode);

 private:
  static std::map<int, std::string> messages;
  static void                       init(void);
  static bool                       is_init;

  HttpStatus(void);
  HttpStatus(const HttpStatus& f);
  HttpStatus& operator=(const HttpStatus& t);
  ~HttpStatus(void);
};
#endif
