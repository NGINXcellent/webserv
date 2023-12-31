/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileReader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 20:51:52 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/02 19:27:37 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FILEREADER_HPP
# define FILEREADER_HPP

#include "../http/HttpStatus.hpp"

#include <map>
#include <string>

struct file_info {
	std::string	fileName;	
	bool		isDir;
	bool		isRegFile;
	long long	lastModified;
	long long	fileSize;
};

class FileReader {
 public:
  static HttpStatusCode getContent(const std::string &fileName, \
                        char **resourceData, long long *resourceSize);	

  static HttpStatusCode getDirContent(const std::string &dirName, \
                           std::map<std::string, struct file_info *> &entries);

 private:
  FileReader(void);
  FileReader(const FileReader& f);
  FileReader& operator=(const FileReader& t);
  ~FileReader(void);
};
#endif
