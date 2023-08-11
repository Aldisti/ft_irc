/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 10:17:01 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/11 14:48:06 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main(void)
{
	Bot	bot(std::string(MYPORT), PASSWORD);
	std::srand((unsigned) std::time(0));
	try
	{
		bot.registerBot();
		bot.launch();
	}
	catch (ErrException &e)
	{
		std::cout << e.what() << std::endl;
	}
	return (0);
}
