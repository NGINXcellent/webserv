/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 12:03:58 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/26 12:40:30 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUTHANDLER_HPP
# define INPUTHANDLER_HPP

class InputHandler {
 private:
  InputHandler(void);
  InputHandler(const InputHandler& f);
  InputHandler& operator=(const InputHandler& t);
  ~InputHandler(void);

 public:
  static bool check_args(int argc, const char **argv);
};

#endif
