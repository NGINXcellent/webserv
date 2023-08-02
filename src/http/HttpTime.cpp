/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpTime.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 22:34:12 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/01 23:12:54 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpTime.hpp"

std::string HttpTime::getCurrentTime(void) {
  std::time_t t = std::time(0);   // get time now
  return (HttpTime::fmtDate(t));
}


std::string HttpTime::fmtDate(time_t miliseconds) {

    std::tm* timeInfo = std::gmtime(&miliseconds);

    char buffer[128];
    std::strftime(buffer, sizeof(buffer), \
                  "%a, %d %b %Y %H:%M:%S GMT\n", timeInfo);

    return std::string(buffer);
}
