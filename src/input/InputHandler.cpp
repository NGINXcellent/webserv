/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 12:05:52 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/26 14:19:40 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/input/InputHandler.hpp"

#include <cstring>
#include <iostream>

bool  InputHandler::check_args(int argc, const char **argv) {
  if ((argc - 1) == 1 && argv[1] != NULL) {
    size_t filenameLen = strlen(argv[1]);
    if (filenameLen > 5 && strncmp(&argv[1][filenameLen - 5], ".conf", 5) == 0)
      return (true);
    else
      return (false);
  }
  return ((argc - 1) == 0);
}
