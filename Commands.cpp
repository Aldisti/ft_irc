/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 09:27:23 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/07 10:35:05 by adi-stef         ###   ########.fr       */
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
	Commands::commands[PRIVMSG] = Commands::privmsgCommand;
	Commands::commands[KILL] = Commands::killCommand;
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
	(void) usr;
	(void) params;
	#ifdef DEBUG
		std::cout << ">> PING command executed" << std::endl;
	#endif
	usr->setWriteBuff(usr->getWriteBuff() + MSG_PING);
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
	usr->setWriteBuff(usr->getWriteBuff() + RPL_YOUREOPER(usr->getNick(), usr->getUser()));
}

void	Commands::privmsgCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	User	*tmp;

	if (usr->getReg() < 7)
		throw (Replies::ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 1)
		throw (Replies::ErrException(ERR_NORECIPIENT(usr->getNick(), usr->getUser(), PRIVMSG).c_str()));
	if (params.size() < 2)
		throw (Replies::ErrException(ERR_NOTEXTTOSEND(usr->getNick(), usr->getUser()).c_str()));
	if (params[0].find('.') != NPOS)
		throw (Replies::ErrException(ERR_BADMASK(usr->getNick(), usr->getUser(), params[0]).c_str()));
	MY_DEBUG(">> trying to find user with nick: " + params[0])
	if ((tmp = srv.getUser(params[0])) == NULL)
		throw (Replies::ErrException(ERR_NOSUCHNICK(usr->getNick(), usr->getUser(), params[0]).c_str()));
	MY_DEBUG(">> user found with nick: " << tmp->getNick())
	tmp->setWriteBuff(tmp->getWriteBuff() + PREFIX(usr->getNick(), usr->getUser()) + " " + PRIVMSG + " " + tmp->getNick() +
			" :" + params[1] + DEL);
	srv.setEvent(tmp->getSockFd(), POLLOUT);
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

