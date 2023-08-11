/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 09:34:51 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/10 12:16:21 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../server/irc.hpp"
#include "Game.hpp"

class	Bot
{
	private:
		// variables
		int					_sfd;
		std::string			_pass;
		char				_buff[MAX_BUFF];
		std::vector<Game *>	_games;

		// functions
		void	botSend(std::string str) const;
		void	botRecv(void);

	public:
		// constructors and destructors
		Bot(std::string port, std::string pass);
		~Bot(void);

		// getters
		std::vector<Game *>	getGames(void) const;
		Game				*getGameByNick(std::string nick) const;

		// setters

		// fuctions
		void	registerBot(void);
		void	launch(void);
};
