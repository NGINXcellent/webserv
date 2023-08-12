/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileReader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 20:32:28 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/10 21:15:11 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/utils/FileReader.hpp"

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

int FileReader::getContent(const std::string &fileName, std::vector<char> *resourceData) {
  std::ifstream inputFile;

  if (access(fileName.c_str(), F_OK) == -1) {
    return 404;
  }

  if (access(fileName.c_str(), R_OK) == -1) {
    return 403;
  }

  inputFile.open(fileName.c_str(), std::ios::binary);

  if (!inputFile.is_open()) {
    std::cout << "Resource not found\n";
    return -1;
  }

  char byte = 0;

  while (inputFile.get(byte)) {
    resourceData->push_back(byte);
  }

  inputFile.close();
  return (0);
}
