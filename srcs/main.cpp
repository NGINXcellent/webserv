/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 13:03:50 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/25 15:44:56 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cstring>

bool  check_args(int argc, const char **argv) {
  if ((argc - 1) == 1) {
    size_t filenameLen = strlen(argv[1]);
    if (filenameLen > 5 && strncmp(&argv[1][filenameLen - 5], ".conf", 5) == 0)
      return (true);
    else
      return (false);
  }
  return ((argc - 1) == 0);
}

// int main(int argc, char **argv) {
//   std::cout << "Hello webserv world" << std::endl;
// }
