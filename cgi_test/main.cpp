/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:26:18 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/10 01:29:07 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

// goal 1: use socket pair to write into a socket
// goal 2: invoke the PHP interpreter and sends it output back to father process
// goal 3: create an epoll queue an insert the socket 
int main(int argc, char **argv) {
	(void)argc;
	(void)argv;
	int sock_pair[2];
	int readSocket = 0;	
	int writeSocket = 1;
	pid_t pid;

	// create a pair of unix socket that transfer data as stream
	// in a non blocking way
	socketpair(AF_LOCAL, SOCK_STREAM | SOCK_NONBLOCK, 0, sock_pair);
	int epollfd = epoll_create(1);

	if (epollfd < 0) {
		std::cout << "Epoll creation error" << std::endl;
	}

	pid = fork();
	
	if (pid == 0) {
		close(sock_pair[readSocket]);
		dup2(sock_pair[writeSocket], STDOUT_FILENO);
		
		char *env_custom[3] = {"USER_NAME=lfarias-", "WEBSERV=webserv-project", NULL};
		char *cmd[3] = {"/bin/php-cgi", "./index.php", NULL};

		if (execve(cmd[0], cmd, env_custom) == -1) {
			std::cerr << "Something went wrong" << std::endl;
			exit(0);
		}
	} else {
		wait(NULL);
		close(sock_pair[writeSocket]);
		ssize_t bytes_read;
		char buffer[4096];	

		while ((bytes_read = read(sock_pair[readSocket], buffer, sizeof(buffer))) > 0) {
			write(STDOUT_FILENO, buffer, bytes_read);
		}

		close(sock_pair[readSocket]);
	}
}
