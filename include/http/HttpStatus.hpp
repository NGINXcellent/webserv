/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 16:13:54 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/25 00:31:47 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPSTATUS_HPP
# define HTTPSTATUS_HPP

#include <string>
#include <map>

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
