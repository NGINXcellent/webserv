/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileWriter.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 22:05:59 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/07 15:29:00 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEWRITER_HPP
# define FILEWRITER_HPP

#include <string>

#include "../http/HttpStatus.hpp"

class FileWriter {
 public:
   static HttpStatusCode writeToFile(const std::string &filename, \
                                   const std::string &contents);
};
#endif
