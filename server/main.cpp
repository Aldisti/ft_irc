/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:17:48 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/11 14:32:22 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "User.hpp"
#include "Server.hpp"

int	main(int argc, char *argv[]) {
	Server	*srv;

	if (argc != 3)
	{
		std::cerr << "Usage error: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	else if (!ft_isdigit(std::string(argv[1])) || std::atoi(argv[1]) < 1024 || strcmp(argv[1], "2147483657") > 0)
	{
		std::cerr << "Error: invalid port number" << std::endl;
		return (1);
	}
	else if (std::string(argv[2]).length() > MAX_BUFF - 5)
	{
		std::cerr << "Error: invalid password" << std::endl;
		return (1);
	}
	else if (signal(SIGINT, SIG_IGN) == SIG_ERR)
	{
		std::cerr << "An error occured when setting signal handling" << std::endl;
		return (1);
	}
	Commands::initCommands();
	try
	{
		srv = new Server(argv[1], std::string(argv[2]));
		srv->polling();
	}
	catch (ErrException &e)
	{
		delete srv;
	}
}
