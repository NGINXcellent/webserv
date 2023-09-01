/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileReader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 20:51:52 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/31 20:28:23 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FILEREADER_HPP
# define FILEREADER_HPP

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
  static int getContent(const std::string &fileName, \
                        char **resourceData, long long *resourceSize);	

  static int getDirContent(const std::string &dirName, \
                           std::map<std::string, struct file_info *> &entries);

 private:
  FileReader(void);
  FileReader(const FileReader& f);
  FileReader& operator=(const FileReader& t);
  ~FileReader(void);
};
#endif
