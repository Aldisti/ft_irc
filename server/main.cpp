/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:17:48 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/10 10:19:16 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "User.hpp"
#include "Server.hpp"

int	main(void) {
	Server	*srv;

//	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
//	{
//		std::cerr << "An error occured when setting signal handling" << std::endl;
//		return (1);
//	}
	Commands::initCommands();
	try
	{
		srv = new Server(PASSWORD);
		srv->polling();
	}
	catch (ErrException &e)
	{
		delete srv;
	}
}
