/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:23:14 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/28 22:23:11 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP

#include "./HttpRequest.hpp"
#include "./HttpResponse.hpp"

class Server {
 public:
  static void  execute(HttpRequest *request, HttpResponse *response);
  static void  startupAndListen(void);

 private:
  Server(void);
  Server(const Server& f);
  Server& operator=(const Server& t);
  ~Server(void);
};
#endif
