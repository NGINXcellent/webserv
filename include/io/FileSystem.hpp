/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileSystem.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 20:18:32 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/31 20:26:40 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILESYSTEM_HPP 
# define FILESYSTEM_HPP

#include <string>
// The goal of this Class is to house helper functions for File Management 

class FileSystem {
public:
  static bool isDirectory(const std::string &filename);

  static int check(const std::string &filename, int conditions);
};
#endif 
