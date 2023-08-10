/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 09:36:35 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/10 15:44:00 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot(std::string port, std::string pass): _pass(pass)
{
	struct sockaddr_in	their_addr;
	struct hostent		*hostname;

	std::cout << "Constructor called" << std::endl;
	memset((void *) &their_addr, 0, sizeof(struct sockaddr_in));	//ft_memset
	this->_sfd = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "Socket called" << std::endl;
	if (this->_sfd == -1)
		throw (Server::ExceptionSocket());
	hostname = gethostbyname(IP.c_str());
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(atoi(port.c_str()));
	their_addr.sin_addr.s_addr = *(unsigned long *)hostname->h_addr;
	if (connect(this->_sfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr_in)))
		throw (Server::ExceptionSocket()); // throw something new
}

void	Bot::botSend(std::string str) const
{
	if (send(this->_sfd, str.c_str(), str.length(), 0) < 0)
		throw (std::exception()); // throw something new
}

void	Bot::botRecv(void)
{
	if (recv(this->_sfd, this->_buff, MAX_BUFF, 0) < 0)
		throw (std::exception()); // throw something new
}

void	Bot::registerBot(void)
{
	std::vector<std::string>	replies;
	std::vector<std::string>	reply;

	this->botSend(BMSG_PASS(this->_pass));
	this->botSend(BMSG_SERVICE(BOT_NAME, BOT_INFO));
	this->botRecv();
	replies = ft_split(std::string(this->_buff), DEL);
	for (int i = 0; i < (int) replies.size(); i++)
	{
		reply = ft_parse(replies[i]);
		if (std::atoi(reply[1].c_str()) > 400)
			throw (std::exception()); //throw something
	}
}

void	Bot::launch(void)
{
	std::vector<std::string>	replies;
	std::vector<std::string>	reply;

	while (true)
	{
		memset(this->_buff, 0, MAX_BUFF);
		this->botRecv();
		replies = ft_split(std::string(this->_buff), DEL);
		for (int i = 0; i < (int) replies.size(); i++)
		{
			reply = ft_parse(replies[i]);
			if (reply[1] != PRIVMSG && reply[1] != ERROR && reply[0] != PING)
				continue ;
			if (reply[0] == PING)
				this->botSend(MSG_PONG(IP));
			else if (reply[1] == ERROR)
				throw (std::exception()); //throw something
			else
			{
				this->botSend(BMSG_PRIVMSG(std::string("gpanico"), std::string("     1    2    3 ")));
				this->botSend(BMSG_PRIVMSG(std::string("gpanico"), std::string("1   x  I o  I  x ")));
				this->botSend(BMSG_PRIVMSG(std::string("gpanico"), std::string("     ―+―+― ")));
				this->botSend(BMSG_PRIVMSG(std::string("gpanico"), std::string("2       I o  I   ")));
				this->botSend(BMSG_PRIVMSG(std::string("gpanico"), std::string("     ―+―+― ")));
				this->botSend(BMSG_PRIVMSG(std::string("gpanico"), std::string("3   o  I     I  x ")));
			}
		}
	}
}

Bot::~Bot()
{
	close(this->_sfd);
	return ;
}
