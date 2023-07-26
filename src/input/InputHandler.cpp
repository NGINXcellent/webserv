/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 12:05:52 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/26 13:30:18 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/input/InputHandler.hpp"

#include <cstring>

bool  InputHandler::check_args(int argc, const char **argv) {
  if ((argc - 1) == 1) {
    size_t filenameLen = strlen(argv[1]);
    if (filenameLen > 5 && strncmp(&argv[1][filenameLen - 5], ".conf", 5) == 0)
      return (true);
    else
      return (false);
  }
  return ((argc - 1) == 0);
}
