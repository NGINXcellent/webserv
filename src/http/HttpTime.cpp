/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpTime.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 22:34:12 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/08 07:13:41 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpTime.hpp"

#include <sys/stat.h>

#include <sstream>
#include <ctime>
#include <iostream>

long long parseToMillis(std::string dateTimeStr);

std::string HttpTime::getCurrentTime(void) {
  std::time_t t = std::time(0);   // get time now
  return (HttpTime::fmtDate(t));
}


std::string HttpTime::fmtDate(time_t milliseconds) {
  std::tm* timeInfo = std::gmtime(&milliseconds);

  if (!timeInfo) {
    // Tratamento de erro, se necessário
    return std::string(); // Retorna uma string vazia em caso de erro
  }

  char buffer[128];
  std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT\n", timeInfo);
  return std::string(buffer);
}

std::string HttpTime::fmtModifiedTime(time_t miliseconds) {
  std::tm* timeInfo = std::gmtime(&miliseconds);
  char buffer[128];
  std::strftime(buffer, sizeof(buffer), \
                "%d-%b-%Y %H:%M",timeInfo);
  return std::string(buffer);
}

std::string HttpTime::getLastModifiedTime(const std::string& filename) {
  struct stat fileInfo;

  if (stat(filename.c_str(), &fileInfo) == 0) {
    time_t modifiedTime = fileInfo.st_mtime;
    return (HttpTime::fmtDate(modifiedTime));
  }

  return ("");
}

bool HttpTime::isModifiedSince(std::string dateTimeStr, std::string filename) {
  long long timestampCheck = parseToMillis(dateTimeStr);

  if (timestampCheck == -1)
    return (true);  // in case of an invalid date, send file

  struct stat fileInfo;
  time_t modifiedTime;

  if (stat(filename.c_str(), &fileInfo) == 0)
    modifiedTime = fileInfo.st_mtime;
  else
    modifiedTime = 0;

  long long modifiedTimeMillis = static_cast<long long>(modifiedTime) * 1000;
  return (modifiedTimeMillis > timestampCheck);
}

int getMonthNumber(const std::string &monthName) {
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", \
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    for (int i = 0; i < 12; ++i) {
        if (monthName == months[i]) {
            return i;
        }
    }

    return -1;  // Invalid month name
}

long long parseToMillis(std::string dateTimeStr) {
  std::cout << "If-Modified-Since: " << dateTimeStr << std::endl;

  if (dateTimeStr == "")
    return (-1);

  std::tm dateTime = {};
  std::istringstream ss(dateTimeStr);
  std::string dayName, monthName;

  // syntax: <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
  ss >> dayName >> dateTime.tm_mday >> monthName >> dateTime.tm_year;
  dateTime.tm_year -= 1900;  // tm_year is years since 1900
  dateTime.tm_mon = getMonthNumber(monthName);
  char delim;
  ss >> dateTime.tm_hour >> delim;
  ss >> dateTime.tm_min >> delim;
  ss >> dateTime.tm_sec;

  if (ss.fail()) {
    ss.str(dateTimeStr);
    ss.clear();

    // example: Friday, 31-Dec-99 23:59:59 GMT
    ss >> dayName >> dateTime.tm_mday >> delim >> monthName >> delim >> dateTime.tm_year;
    dateTime.tm_mon = getMonthNumber(monthName);
    ss >> dateTime.tm_hour >> delim;
    ss >> dateTime.tm_min >> delim;
    ss >> dateTime.tm_sec;
  }

  if (ss.fail()) {
    ss.str(dateTimeStr);
    ss.clear();

    // example: Fri Dec 31 23:59:59 1999
    ss >> dayName >> monthName >> dateTime.tm_mday;
    dateTime.tm_mon = getMonthNumber(monthName);
    ss >> dateTime.tm_hour >> delim;
    ss >> dateTime.tm_min >> delim;
    ss >> dateTime.tm_sec;
    ss >> dateTime.tm_year;
    dateTime.tm_year -= 1900;
  }

  if (ss.fail()) {
    return (-1);
  }

  dateTime.tm_isdst = 0;  // forces maketime to consider time as GMT
  std::time_t timestamp = std::mktime(&dateTime);
  timestamp = timestamp - timezone;

  if (timestamp == -1) {
    std::cout << "error converting timestamp" << std::endl;
    return -1;
  }

  long long milliseconds = static_cast<long long>(timestamp) * 1000;

  return (milliseconds);
}

long long HttpTime::getCurrentTimeInMillis(void) {
  std::time_t currentTime = std::time(0);   // get time now
  long long milliseconds = static_cast<long long>(currentTime) * 1000;
  return milliseconds;  
}
