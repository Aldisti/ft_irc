/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 09:34:51 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/11 14:11:03 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../server/irc.hpp"
#include "Game.hpp"
#define	START std::string("start")
#define	ACCEPT std::string("accept")
#define P2_REQUEST std::string("???")

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
		void	botSend(std::vector<std::string> msg) const;
		void	botRecv(void);
		void	addGame(Game *game);
		void	removeGame(Game *game);

	public:
		// constructors and destructors
		Bot(char *port, std::string pass);
		~Bot(void);

		// getters
		std::vector<Game *>	getGames(void) const;
		Game				*getGameByNick(std::string nick) const;

		// setters

		// fuctions
		void	registerBot(void);
		void	launch(void);
		void	startGame(std::string nick, std::vector<std::string> reply);
		void	newRequest(std::string nick, Game *game, std::vector<std::string> reply);
		void	continueGame(std::string nick, Game *game, std::vector<std::string> reply);
};
