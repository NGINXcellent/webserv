/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeType.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 12:02:25 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/29 14:01:32 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MIMETYPE_HPP
# define MIMETYPE_HPP

#include <string>
#include <map>

class MimeType {
 public:
  static std::string identify(const std::string &str);

 private:
  static std::map<std::string, std::string> types;
  static bool is_set;

  MimeType(void);
  MimeType(const MimeType& f);
  MimeType& operator=(const MimeType& t);
  ~MimeType(void);

  static void init_table(void);
};
#endif
