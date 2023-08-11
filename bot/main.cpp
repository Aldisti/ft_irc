/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 10:17:01 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/11 14:32:06 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage error: ./ircbot <port> <password>" << std::endl;
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
	std::srand((unsigned) std::time(0));
	try
	{
		Bot	bot(argv[1], std::string(argv[2]));
		bot.registerBot();
		bot.launch();
	}
	catch (ErrException &e)
	{
		std::cout << e.what() << std::endl;
	}
	return (0);
}
