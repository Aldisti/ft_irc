/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:17:48 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/08 11:20:17 by gpanico          ###   ########.fr       */
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
	srv = new Server("123");
	srv->polling();
	delete srv;
}
