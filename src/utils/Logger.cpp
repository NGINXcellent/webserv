/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:11:56 by lfarias-          #+#    #+#             */
/*   Updated: 2023/11/09 14:18:13 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/utils/Logger.hpp"

#include <string>

std::stringstream Logger::msg; //= std::stringstream();

void Logger::logMessage(const std::string &msg, LogLevel level) {
  std::string logMsg = "";
  int fd = 0;

  switch(level) {
    case Info:
      logMsg += "[INFO]:\t";
      break;
    case Warning:
      logMsg += "[WARNING]:\t";
      fd = 1;
      break;
    case Error:
      logMsg += "[ERROR]:\t";
      fd = 1;
      break;
    case Debug:
      logMsg += "[DEBUG]:\t";
  }

  logMsg += msg;

  if (fd != 0)
    std::cerr << logMsg << std::endl;
  else
    std::cout << logMsg << std::endl;

  Logger::msg.str("");
}

void Logger::print(LogLevel level) {
  Logger::logMessage(Logger::msg.str(), level);
}
