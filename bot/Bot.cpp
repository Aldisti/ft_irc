/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 09:36:35 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/11 15:09:46 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot(char *port, std::string pass): _pass(pass)
{
	struct sockaddr_in	their_addr;
	struct hostent		*hostname;

	Utils::ft_memset((void *) &their_addr, 0, sizeof(struct sockaddr_in));
	this->_sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sfd == -1)
		throw (ErrException("socket failed"));
	hostname = gethostbyname(IP.c_str());
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(std::atoi(port));
	their_addr.sin_addr.s_addr = *(unsigned long *)hostname->h_addr;
	if (connect(this->_sfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr_in)))
		throw (ErrException("connect() failed"));
}

void	Bot::botSend(std::string str) const
{
	if (send(this->_sfd, str.c_str(), str.length(), 0) <= 0)
		throw (ErrException("send() failed"));
}

void	Bot::botSend(std::vector<std::string> msg) const
{
	for (int i = 0; i < (int) msg.size(); i++)
		this->botSend(msg[i]);
}

void	Bot::botRecv(void)
{
	if (recv(this->_sfd, this->_buff, MAX_BUFF, 0) <= 0)
		throw (ErrException("recv() failed"));
}

void	Bot::addGame(Game *game)
{
	this->_games.push_back(game);
}

void	Bot::removeGame(Game *game)
{
	for (int i = 0; i < (int) this->_games.size(); i++)
		if (game == this->_games[i])
		{
			delete this->_games[i];
			this->_games.erase(this->_games.begin() + i);
			break ;
		}
}

std::vector<Game *>	Bot::getGames(void) const
{
	return (this->_games);
}

Game	*Bot::getGameByNick(std::string nick) const
{
	for (int i = 0; i < (int) this->_games.size(); i++)
		if (nick == this->_games[i]->getP1() || nick == this->_games[i]->getP2())
			return (this->_games[i]);
	return (NULL);
}

void	Bot::registerBot(void)
{
	std::vector<std::string>	replies;
	std::vector<std::string>	reply;

	this->botSend(BMSG_PASS(this->_pass));
	this->botSend(BMSG_SERVICE(BOT_NAME, BOT_INFO));
	this->botRecv();
	replies = Utils::Utils::ft_split(std::string(this->_buff), DEL);
	for (int i = 0; i < (int) replies.size(); i++)
	{
		reply = Utils::Utils::ft_parse(replies[i]);
		if (std::atoi(reply[1].c_str()) > 400)
			throw (ErrException("registration to server failed"));
	}
}

void	Bot::launch(void)
{
	std::vector<std::string>	replies;
	std::vector<std::string>	reply;
	std::string					nick;
	std::string					errorMessage;
	Game						*game;

	while (true)
	{
		Utils::ft_memset(this->_buff, 0, MAX_BUFF);
		this->botRecv();
		replies = Utils::Utils::ft_split(std::string(this->_buff), DEL);
		for (int i = 0; i < (int) replies.size(); i++)
		{
			reply = Utils::Utils::ft_parse(replies[i]);
			MY_DEBUG(std::string(">> rep: ") << replies[i])
			if (reply[1] != PRIVMSG && reply[1] != ERROR && reply[1] != "401" && reply[0] != PING)
				continue ;
			if (reply[0] == PING)
				this->botSend(MSG_PONG(IP));
			else if (reply[1] == ERROR)
				throw (ErrException("ERROR: connection with server lost"));
			else if (reply[1] == "401")
			{
				reply[3] = Utils::Utils::ft_tolower(reply[3]);
				while ((game = this->getGameByNick(reply[3])) != NULL)
				{
					nick = game->getP1() == reply[3] ? game->getP2() : game->getP1();
					this->botSend(BMSG_PRIVMSG(nick, std::string(reply[3] + " has left the game")));
					this->removeGame(game);
				}
			}
			else
			{
				nick = Utils::Utils::ft_tolower(reply[0].substr(0, reply[0].find('!')));
				game = this->getGameByNick(nick);
				try
				{
					if (game == NULL)
						this->startGame(nick, reply);
					else if (game->getP2().substr(0, (int) P2_REQUEST.size()) == P2_REQUEST)
						this->newRequest(nick, game, reply);
					else
						this->continueGame(nick, game, reply);
				}
				catch (ErrException &e)
				{
					errorMessage = std::string(e.what()).substr(0, 4);
					if (errorMessage == "send" || errorMessage == "recv")
						throw (ErrException(e.what()));
					this->botSend(BMSG_PRIVMSG(nick, std::string(e.what())));
				}
			}
		}
	}
}

void	Bot::startGame(std::string nick, std::vector<std::string> reply)
{
	std::vector<std::string>	params;
	Game						*game;

	params = Utils::Utils::ft_split(reply[reply.size() - 1], " ");
	if (params[0] != START && params[0] != ACCEPT)
		throw (ErrException("Invalid command: usage -> :start [second_player]/:accept"));
	if (params.size() == 1 && params[0] == START)
	{
		game = new Game(nick, "");
		this->addGame(game);
		this->botSend(game->getMatPrint(game->getP1()));
		this->botSend(BMSG_PRIVMSG(nick, std::string("make your move")));
	}
	else if (params.size() == 1 && params[0] == ACCEPT)
	{
		game = this->getGameByNick(P2_REQUEST + nick);
		if (!game)
			throw (ErrException("Invalid command: the request has been rejected"));
		game->setP2(nick);
		this->botSend(game->getMatPrint(game->getP1()));
		this->botSend(BMSG_PRIVMSG(game->getP1(), std::string("Make your move")));
		this->botSend(game->getMatPrint(game->getP2()));
		this->botSend(BMSG_PRIVMSG(nick, std::string("Wait your turn")));
	}
	else
	{
		params[1] = Utils::Utils::ft_tolower(params[1]);
		if (this->getGameByNick(params[1]))
			throw (ErrException((params[1] + std::string(" is already playing")).c_str()));
		game = new Game(nick, P2_REQUEST + params[1]);
		this->addGame(game);
		this->botSend(BMSG_PRIVMSG(nick, std::string("The request has been sent")));
		this->botSend(BMSG_PRIVMSG(params[1], std::string("Do you wanna play with " + nick + "?")));
	}
}

void	Bot::newRequest(std::string nick, Game *game, std::vector<std::string> reply)
{
	std::vector<std::string>	params;

	params = Utils::Utils::ft_split(reply[reply.size() - 1], " ");
	if (params[0] != START && params[0] != ACCEPT)
		throw (ErrException("Invalid command: usage -> :start [second_player]/:accept"));
	if (params.size() == 1 && params[0] == START)
	{
		game->setP2("");
		this->botSend(game->getMatPrint(game->getP1()));
		this->botSend(BMSG_PRIVMSG(nick, std::string("Make your move")));
	}
	else
	{
		params[1] = Utils::Utils::ft_tolower(params[1]);
		if (this->getGameByNick(params[1]))
			throw (ErrException((params[1] + std::string(" is already playing")).c_str()));
		game->setP2(P2_REQUEST + params[1]);
		this->botSend(BMSG_PRIVMSG(nick, std::string("The request has been sent")));
		this->botSend(BMSG_PRIVMSG(params[1], std::string("Do you wanna play with " + nick + "?")));
	}
}

void	Bot::continueGame(std::string nick, Game *game, std::vector<std::string> reply)
{
	std::vector<std::string>	params;

	if (!game->isNickTurn(nick))
		throw (ErrException("Wait your turn"));
	params = Utils::Utils::ft_split(reply[reply.size() - 1], " ");
	if (params.size() < 2)
		throw (ErrException("Invalid command: usage -> <row> <col>"));
	if (!Utils::Utils::ft_isdigit(params[0]) || !Utils::Utils::ft_isdigit(params[1]))
		throw (ErrException("Invalid command: params must be numbers"));
	game->makeMove(std::atoi(params[0].c_str()), std::atoi(params[1].c_str()));
	if (game->getEnd() && game->checkVictory())
	{
		this->botSend(game->getMatPrint(game->getP1()));
		this->botSend(BMSG_PRIVMSG(nick, std::string("YOU WIN")));
		if (game->getP2() != "")
		{
			this->botSend(game->getMatPrint(game->getP2()));
			this->botSend(BMSG_PRIVMSG(
						(game->getP2() == nick ? game->getP1() : game->getP1()), std::string("YOU LOSE")));
		}
	}
	else if (game->getEnd())
	{
		this->botSend(game->getMatPrint(game->getP1()));
		this->botSend(BMSG_PRIVMSG(game->getP1(), std::string("DRAW")));
		if (game->getP2() != "")
		{
			this->botSend(game->getMatPrint(game->getP2()));
			this->botSend(BMSG_PRIVMSG(game->getP2(), std::string("DRAW")));
		}
	}
	else if (game->getP2() == "")
	{
		game->botMove();
		if (game->getEnd())
		{
			this->botSend(game->getMatPrint(game->getP1()));
			this->botSend(BMSG_PRIVMSG(nick, std::string("YOU LOSE")));
			this->botSend(BMSG_PRIVMSG(nick, BOT_NAME + std::string(" says: skill issue, kys ;)")));
		}
	}
	if (!game->getEnd())
	{
		this->botSend(game->getMatPrint(game->getP1()));
		this->botSend(BMSG_PRIVMSG(game->getP1(), std::string(game->getTurn() == 1 ? "Make your move" : "Wait your turn")));
		if (game->getP2() != "")
		{
			this->botSend(game->getMatPrint(game->getP2()));
			this->botSend(BMSG_PRIVMSG(game->getP2(), std::string(game->getTurn() == 2 ? "Make your move" : "Wait your turn")));
		}
	}
	if (game->getEnd())
		this->removeGame(game);
}

Bot::~Bot()
{
	for (int i = 0; i < (int) this->_games.size(); i++)
		delete this->_games[i];
	close(this->_sfd);
	return ;
}
