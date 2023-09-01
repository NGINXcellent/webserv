/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileSystem.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 20:27:58 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/31 21:14:56 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/io/FileSystem.hpp"

#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>

int FileSystem::check(const std::string &filename, int conditions) {
  if (access(filename.c_str(), conditions) == 0) {
    return (0);
  }

  if (errno == EACCES|| errno == EPERM) {
    return (403);
  } else if (errno == ENOENT) {
    return (404);
  } else {
    return (500);
  }
}

bool FileSystem::isDirectory(const std::string &filename) {
  struct stat fileInfo;
  return (stat(filename.c_str(), &fileInfo) == 0) && S_ISDIR(fileInfo.st_mode);
}
