/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileReader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 20:51:52 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/10 20:54:56 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FILEREADER_HPP
# define FILEREADER_HPP

#include <vector>
#include <string>

class FileReader {
 public:
  static int getContent(const std::string &fileName, \
                        std::vector<char> *resourceData);
 private:
  FileReader(void);
  FileReader(const FileReader& f);
  FileReader& operator=(const FileReader& t);
  ~FileReader(void);
};
#endif
