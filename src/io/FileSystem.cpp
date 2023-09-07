/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileSystem.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 20:27:58 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/05 13:24:01 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/io/FileSystem.hpp"

#include <unistd.h>
#include <sys/stat.h>

#include <cerrno>

#include "../../include/http/HttpStatus.hpp" // status code

HttpStatusCode FileSystem::check(const std::string &filename, int conditions) {
  if (access(filename.c_str(), conditions) == 0) {
    return (Ready);
  }

  if (errno == EACCES || errno == EPERM) {
    return (Forbidden);
  } else if (errno == ENOENT) {
    return (Not_Found);
  } else {
    return (Internal_Server_Error);
  }
}

bool FileSystem::isDirectory(const std::string &filename) {
  struct stat fileInfo;
  return (stat(filename.c_str(), &fileInfo) == 0) && S_ISDIR(fileInfo.st_mode);
}
