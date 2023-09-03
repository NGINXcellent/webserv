/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileReader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 20:32:28 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/03 15:42:13 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/io/FileReader.hpp"

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include <algorithm>  // sort
#include <fstream>    // IO file reading
#include <iostream>   // cout

#include "../../include/http/HttpStatus.hpp" // status codes

bool isRegularFile(const std::string &filename);
int getFileInfo(const std::string &filename, struct file_info *info);

HttpStatusCode FileReader::getContent(const std::string &fileName, \
                                      char **resourceData, \
                                      long long *resourceSize) {
  if (access(fileName.c_str(), F_OK) == -1) {
    return Not_Found;
  }

  if (access(fileName.c_str(), R_OK) == -1) {
    return Forbidden;
  }

  if (!isRegularFile(fileName.c_str())) {
    return (Not_Found);
  }

  std::ifstream inputFile;
  inputFile.open(fileName.c_str(), std::ios::binary);

  if (!inputFile.is_open()) {
    std::cout << "Resource not found\n";
    return Internal_Server_Error;
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
  return (Ready);
}

HttpStatusCode FileReader::getDirContent(const std::string &dirName, \
                              std::map<std::string, struct file_info *> &entries) {

  DIR* currentDir = opendir(dirName.c_str());

  if (currentDir != NULL) {
    struct dirent *entry;
    
    while (true) {
      entry = readdir(currentDir);
      
      if (entry == NULL)  {
        closedir(currentDir);
        break;
      }
      
      std::string filename = entry->d_name;

      if (filename == ".") {
        continue;
      }
      
      std::string filepath = dirName + "/" + filename; 
      struct file_info *info = new struct file_info;
      info->fileName = entry->d_name;

      if (getFileInfo(filepath.c_str(), info) != 0) {
        return (Internal_Server_Error);
      }

      entries.insert(std::make_pair(info->fileName, info));
    }
  } else {
    return (Internal_Server_Error);
  }

  return (Ready);
}

bool isRegularFile(const std::string &filename) {
    struct stat fileInfo;
    return (stat(filename.c_str(), &fileInfo) == 0) && S_ISREG(fileInfo.st_mode);
}

int getFileInfo(const std::string &filename, struct file_info *info) {
  struct stat fileDetails;

  if (stat(filename.c_str(), &fileDetails) != 0) {
    return (-1);
  }
  
  info->isDir = S_ISDIR(fileDetails.st_mode); 
  info->isRegFile = S_ISREG(fileDetails.st_mode);
  info->lastModified = fileDetails.st_mtime;
  info->fileSize = fileDetails.st_size;
  return (Ready);
}

