/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:11:56 by lfarias-          #+#    #+#             */
/*   Updated: 2023/11/09 15:46:58 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/utils/Logger.hpp"

#include <string>

std::stringstream Logger::msg; //= std::stringstream();

void Logger::logMessage(const std::string &msg, LogLevel level) {
  std::string levelStr = "";
  std::string colorCode;
  int fd = 1;

  switch(level) {
    case Info:
      levelStr = "[INFO]:\t";
      colorCode = "\033[32m";
      break;
    case Warning:
      levelStr = "[WARNING]:\t";
      colorCode = "\033[33m";
      fd = 2;
      break;
    case Error:
      levelStr = "[ERROR]:\t";
      colorCode = "\033[31m";
      fd = 2;
      break;
    case Debug:
      levelStr = "[DEBUG]:\t";
      colorCode = "\033[36m";
  }

  if (fd != 1)
    std::cerr << colorCode << levelStr << msg << "\033[0m" << std::endl;
  else
    std::cout << colorCode << levelStr << msg << "\033[0m" << std::endl;

  Logger::msg.str("");
}

void Logger::print(LogLevel level) {
  Logger::logMessage(Logger::msg.str(), level);
}
