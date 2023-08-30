/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpTime.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 22:38:10 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/29 21:17:17 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPTIME_HPP
# define HTTPTIME_HPP

#include <string>
#include <ctime>

class HttpTime {
 public:
  static std::string getCurrentTime(void);
  static std::string fmtDate(time_t miliseconds);
  static std::string fmtModifiedTime(time_t miliseconds);
  static std::string getLastModifiedTime(const std::string& filename);
  static long long   getCurrentTimeInMillis(void);
  static bool        isModifiedSince(std::string dateTimeStr, std::string filename);

 private:
  HttpTime(void);
  HttpTime(const HttpTime& f);
  HttpTime& operator=(const HttpTime& t);
  ~HttpTime(void);
};
#endif
