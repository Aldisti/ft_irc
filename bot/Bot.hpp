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

class	Bot
{
	private:
		int				_sfd;
		std::string		_pass;
		char			_buff[MAX_BUFF];

	public:
		Bot(std::string port, std::string pass);
		~Bot(void);
		void	registerBot(void);
		void	launch(void);

	private:
		void	botSend(std::string str) const;
		void	botRecv(void);
};
