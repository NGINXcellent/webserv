/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:03:20 by lfarias-          #+#    #+#             */
/*   Updated: 2023/11/09 14:12:04 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
# define LOGGER_HPP

#include <iostream>
#include <sstream>

enum LogLevel {
	Info,
	Warning,
	Error,
	Debug
};

class Logger {
public:
	static std::stringstream msg;
	static void logMessage(const std::string &msg, LogLevel level);
	static void print(LogLevel level);
};
#endif
