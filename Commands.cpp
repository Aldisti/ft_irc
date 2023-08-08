/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 09:27:23 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/08 09:41:40 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

std::map<std::string, Commands::cmd_p >	Commands::commands;

void	Commands::initCommands(void)
{
	Commands::commands[CAP] = Commands::capCommand;
	Commands::commands[PASS] = Commands::passCommand;
	Commands::commands[NICK] = Commands::nickCommand;
	Commands::commands[USER] = Commands::userCommand;
	Commands::commands[PING] = Commands::pongCommand;
	Commands::commands[PONG] = Commands::pingCommand;
	Commands::commands[QUIT] = Commands::errorCommand;
	Commands::commands[OPER] = Commands::operCommand;
	Commands::commands[MODE] = Commands::modeCommand;
	Commands::commands[PRIVMSG] = Commands::privmsgCommand;
	Commands::commands[WALLOPS] = Commands::wallopsCommand;
	Commands::commands[SQUIT] = Commands::squitCommand;
	Commands::commands[KILL] = Commands::killCommand;
	Commands::commands[AWAY] = Commands::awayCommand;
}

void	Commands::passCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	if (params.size() == 0)
		throw (Replies::ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), PASS).c_str()));
	if (usr->getReg() != 0)
		throw (Replies::ErrException(ERR_ALREADYREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params[0] != srv.getPass())
		throw (Replies::ErrException(ERR_PASSWDMISMATCH(usr->getNick(), usr->getUser()).c_str()));
	#ifdef DEBUG
		std::cout << ">> PASS command executed\nuser->sfd [" << usr->getSockFd() << "]" << std::endl;
	#endif
	usr->setReg(1);
}

void	Commands::capCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	(void) usr;
	(void) params;
	(void) srv;
	if (params[0] == "LS")
		usr->setWriteBuff(usr->getWriteBuff() + MSG_CAP);
	#ifdef DEBUG
		std::cout << ">> CAP command executed" << std::endl;
	#endif
}

void	Commands::nickCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	if (usr->getReg() % 2 == 0)
		throw (Replies::ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() == 0)
		throw (Replies::ErrException(ERR_NONICKNAMEGIVEN(usr->getNick(), usr->getUser()).c_str()));
	if (!User::checkNick(params[0]))
		throw (Replies::ErrException(ERR_ERRONEUSNICKNAME(usr->getNick(), usr->getUser()).c_str()));
	if (srv.getUser(params[0]))
		throw (Replies::ErrException(ERR_NICKNAMEINUSE(usr->getNick(), usr->getUser()).c_str()));
	usr->setNick(params[0]);
	usr->setReg(usr->getReg() | 2);
	#ifdef DEBUG
		std::cout << ">> NICK command executed" << std::endl
		<< "usr->sfd [" << usr->getSockFd() << "] "
		<< "usr->nick [" << usr->getNick() << "] " << std::endl;
	#endif
	if (usr->getReg() >= 7) {
		usr->setWriteBuff(usr->getWriteBuff() + RPL_WELCOME(usr->getNick(), usr->getUser(), SRV_NAME));
		#ifdef DEBUG
			std::cout << ">> USER registered correctly from NICK command" << std::endl;
		#endif
	}
}

void	Commands::userCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	(void) srv;
	if (usr->getReg() % 2 == 0)
		throw (Replies::ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 4)
		throw (Replies::ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), PASS).c_str()));
	if ((usr->getReg() >> 2) % 2)
		throw (Replies::ErrException(ERR_ALREADYREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	usr->setUser(params[0]);
	usr->setMode(atoi(params[1].c_str())); // ft_atoi
	usr->setReal(params[3]);
	usr->setReg(usr->getReg() | 4);
	#ifdef DEBUG
		std::cout << ">> USER command executed" << std::endl
		<< "usr->sfd [" << usr->getSockFd() << "] "
		<< "usr->user [" << usr->getUser() << "] "
		<< "usr->real [" << usr->getReal() << "] " << std::endl;
	#endif
	if (usr->getReg() >= 7) {
		usr->setWriteBuff(usr->getWriteBuff() + RPL_WELCOME(usr->getNick(), usr->getUser(), SRV_NAME));
		#ifdef DEBUG
			std::cout << ">> USER registered correctly from USER command" << std::endl;
		#endif
	}
}

void	Commands::pingCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	(void) srv;
	(void) params;
	#ifdef DEBUG
		std::cout << ">> PING command executed" << std::endl;
	#endif
	usr->resetTime();
	usr->setPing(false);
}

void	Commands::pongCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	(void) srv;
	if (params[0] != IP && params[0] != SRV_NAME)
		throw (Replies::ErrException(ERR_NOSUCHSERVER(usr->getNick(), usr->getUser(), params[0]).c_str()));
	#ifdef DEBUG
		std::cout << ">> PONG command executed" << std::endl;
	#endif
	usr->setWriteBuff(usr->getWriteBuff() + MSG_PONG(IP));
}

void	Commands::errorCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	std::string	message;

	(void)	srv;
	message = params.size() == 0 ? std::string("") : params[0];
	usr->setClose(true);
	usr->setWriteBuff(usr->getWriteBuff() + MSG_ERROR(message));
}

void	Commands::operCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	(void) srv;
	if (usr->getReg() < 7)
		throw (Replies::ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 2)
		throw (Replies::ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), OPER).c_str()));
	if (params[1] != OPER_PASSWORD)
		throw (Replies::ErrException(ERR_PASSWDMISMATCH(usr->getNick(), usr->getUser()).c_str()));
	#ifdef DEBUG
		std::cout << ">> OPER command executed" << std::endl;
		std::cout << ">> usr: sfd [" << usr->getSockFd() << "] is now an operator" << std::endl;
		std::cout << ">> usr user: old [" << usr->getUser()
		<< "] new [" << params[1] << "]" << std::endl;
	#endif
	usr->setUser(params[0]);
	usr->setOperator(true);
	usr->setMode(usr->getMode() | OPERATOR);
	usr->setWriteBuff(usr->getWriteBuff() + RPL_YOUREOPER(usr->getNick(), usr->getUser()));
}

void	Commands::privmsgCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	User	*tmp;
//	Channel	*chn;

	if (usr->getReg() < 7)
		throw (Replies::ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 1)
		throw (Replies::ErrException(ERR_NORECIPIENT(usr->getNick(), usr->getUser(), PRIVMSG).c_str()));
	if (params.size() < 2)
		throw (Replies::ErrException(ERR_NOTEXTTOSEND(usr->getNick(), usr->getUser()).c_str()));
	if (params[0].find('.') != NPOS)
		throw (Replies::ErrException(ERR_BADMASK(usr->getNick(), usr->getUser(), params[0]).c_str()));
	if (CHANNEL.find(params[0][0]) == NPOS)
	{
		MY_DEBUG(">> trying to find user with nick: " + params[0])
		if ((tmp = srv.getUser(params[0])) == NULL)
			throw (Replies::ErrException(ERR_NOSUCHNICK(usr->getNick(), usr->getUser(), params[0]).c_str()));
		MY_DEBUG(">> user found with nick: " << tmp->getNick())
		if (tmp->getMode() & F_AWAY)
		{
			usr->setWriteBuff(usr->getWriteBuff() + RPL_AWAY(usr->getNick(), usr->getUser(), tmp->getNick(), tmp->getAwayMsg()).c_str());
			return ;
		}
		tmp->setWriteBuff(tmp->getWriteBuff() + PREFIX(usr->getNick(), usr->getUser()) + " " + PRIVMSG + " " + tmp->getNick() +
				" :" + params[1] + DEL);
		srv.setEvent(tmp->getSockFd(), POLLOUT);
	}
//	else
//	{
//		
//	}
}

void	Commands::modeCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	char		first;
	std::string	modes = "";

	(void) srv;
	if (usr->getReg() < 7)
		throw (Replies::ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 2)
		throw (Replies::ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), MODE).c_str()));
	if (usr->getNick() != params[0])
		throw (Replies::ErrException(ERR_USERSDONTMATCH(usr->getNick(), usr->getUser()).c_str()));
	for (int i = 1; i < (int) params.size(); i++)
	{
		first = params[i][0];
		if (first == '+' || first == '-') {
			modes.append(1, params[i][0]);
			for (int j = 1; j < (int) params[i].size(); j++) {
				if (VALID_MODES.find(params[i][j]) == NPOS)
					throw (Replies::ErrException(ERR_UMODEUNKNOWNFLAG(usr->getNick(), usr->getUser()).c_str()));
				if (first == '+') {
					if (params[i][j] == 'o' || params[i][j] == 'O')
						continue ;
					usr->setMode(usr->getMode() | ft_convertToMode(params[i][j]));
				}
				else {
					if (params[i][j] == 'o' || params[i][j] == 'O')
						usr->setOperator(false);
					usr->setMode(usr->getMode() & (~ft_convertToMode(params[i][j])));
				}
				modes.append(1, params[i][j]);
			}
			modes.append(1, ' ');
		} else {
			throw (Replies::ErrException(ERR_UMODEUNKNOWNFLAG(usr->getNick(), usr->getUser()).c_str()));
		}
	}
	usr->setWriteBuff(usr->getWriteBuff() + RPL_UMODEIS(usr->getNick(), usr->getUser(), modes));
}

void	Commands::wallopsCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	std::vector<User *>	tmpUsers;

	if (usr->getReg() < 7)
		throw (Replies::ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 1)
		throw (Replies::ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), WALLOPS).c_str()));
	tmpUsers = srv.getUsers();
	for (int i = 0; i < (int) tmpUsers.size(); i++)
	{
		if (tmpUsers[i]->getNick() == usr->getNick() || !(tmpUsers[i]->getMode() & WALLOP))
			continue ;
		tmpUsers[i]->setWriteBuff(tmpUsers[i]->getWriteBuff() + PREFIX(usr->getNick(),
					usr->getUser()) + " " + WALLOPS + " :" + params[0] + DEL);
		srv.setEvent(tmpUsers[i]->getSockFd(), POLLOUT);
	}
}

void	Commands::squitCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	std::vector<User *>	tmpUsers;
	MY_DEBUG("prova")
	if (params.size() < 2)
		throw (Replies::ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), SQUIT).c_str()));
	if (params[0] != IP && params[0] != SRV_NAME)
		throw (Replies::ErrException(ERR_NOSUCHSERVER(usr->getNick(), usr->getUser(), params[0]).c_str()));
	if (!usr->getOperator())
		throw (Replies::ErrException(ERR_NOPRIVILEGES(usr->getNick(), usr->getUser()).c_str()));
	std::vector<std::string>	tmp(params.begin() + 1, params.end());
	tmpUsers = srv.getUsers();
	for (int i = 0; i < (int) tmpUsers.size(); i++)
	{
		tmpUsers[i]->setClose(true);
		tmpUsers[i]->setWriteBuff("");
	}
	Commands::wallopsCommand(srv, usr, tmp);
	srv.setEnd(true);
}

void	Commands::killCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	User	*tmp = NULL;

	MY_DEBUG(">> checking registration status of " << usr->getNick())
	if (usr->getReg() < 7)
		throw (Replies::ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	MY_DEBUG(">> checking operator status of " << usr->getNick())
	if (!usr->getOperator())
		throw (Replies::ErrException(ERR_NOPRIVILEGES(usr->getNick(), usr->getUser()).c_str()));
	MY_DEBUG(">> checking params for KILL")
	if (params.size() < 2)
		throw (Replies::ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), KILL).c_str()));
	MY_DEBUG(">> trying to find user with nick: " + params[0])
	if ((tmp = srv.getUser(params[0])) == NULL)
		throw (Replies::ErrException(ERR_NOSUCHNICK(usr->getNick(), usr->getUser(), params[0]).c_str()));
	MY_DEBUG(">> user found with nick: " << tmp->getNick())
	tmp->setClose(true);
	tmp->setWriteBuff(usr->getWriteBuff() + MSG_KILL(usr->getNick(), params[1]));
	srv.setEvent(tmp->getSockFd(), POLLOUT);
}

void	Commands::awayCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	(void) srv;

	if (usr->getReg() < 7)
		throw (Replies::ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() == 0)
	{
		usr->setAwayMsg(MSG_AWAY);
		usr->setMode(usr->getMode() & (~F_AWAY));
		usr->setWriteBuff(usr->getWriteBuff() + RPL_UNAWAY(usr->getNick(), usr->getUser()).c_str());
	}
	else
	{
		usr->setAwayMsg(params[0]);
		usr->setMode(usr->getMode() | F_AWAY);
		usr->setWriteBuff(usr->getWriteBuff() + RPL_NOWAWAY(usr->getNick(), usr->getUser()).c_str());
	}
}
