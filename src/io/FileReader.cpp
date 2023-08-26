/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileReader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 20:32:28 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/25 19:05:48 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/io/FileReader.hpp"

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

bool isRegularFile(const std::string &filename);

int FileReader::getContent(const std::string &fileName, char **resourceData, long long *resourceSize) {
  if (access(fileName.c_str(), F_OK) == -1) {
    return 404;
  }

  if (access(fileName.c_str(), R_OK) == -1) {
    return 403;
  }

  if (!isRegularFile(fileName.c_str()))
    return (404);

  std::ifstream inputFile;
  inputFile.open(fileName.c_str(), std::ios::binary);

  if (!inputFile.is_open()) {
    std::cout << "Resource not found\n";
    return -1;
  }

  // Determine the size of the file
  inputFile.seekg(0, std::ios::end);
  std::streampos fileSize = inputFile.tellg();
  inputFile.seekg(0, std::ios::beg);

  // Allocate a buffer to hold the entire content
  *resourceData = new char[fileSize];
  *resourceSize = static_cast<long long>(fileSize);
  inputFile.read(*resourceData, fileSize);
  inputFile.close();
  return (0);
}

bool FileReader::isDirectory(const std::string &filename) {
  struct stat fileInfo;

  if (stat(filename.c_str(), &fileInfo) != 0) {
      return false; // Erro ao obter informações do arquivo
  }

  return (true);
}

bool isRegularFile(const std::string &filename) {
    struct stat fileInfo;
    return (stat(filename.c_str(), &fileInfo) == 0) && S_ISREG(fileInfo.st_mode);
}
