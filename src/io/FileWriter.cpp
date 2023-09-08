/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileWriter.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 22:21:51 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/07 17:29:22 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/io/FileWriter.hpp"

#include <fstream>

HttpStatusCode FileWriter::writeToFile(const std::string &filename, \
                                       const std::string &contents) {
  std::ofstream file(filename.c_str(), std::ofstream::out | std::ofstream::trunc);
  HttpStatusCode opStatus = Ready;

  if (file.is_open()) {
    file << contents;
    file.close();
  } else {
    opStatus = Internal_Server_Error;
  }

  return opStatus;
}

