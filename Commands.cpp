/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 09:27:23 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/03 11:51:18 by adi-stef         ###   ########.fr       */
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
}

void	Commands::passCommand(const Server &srv, User *usr, std::vector<std::string> params)
{
	if (params.size() == 0)
		throw (Replies::ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), PASS).c_str()));
	if (usr->getReg() != 0)
		throw (Replies::ErrException(ERR_ALREADYREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params[0] != srv.getPass())
		throw (Replies::ErrException(ERR_INCORRECTPASS(usr->getNick(), usr->getUser()).c_str()));
	#ifdef DEBUG
		std::cout << ">> PASS command executed\nuser->sfd [" << usr->getSockFd() << "]" << std::endl;
	#endif
	usr->setReg(1);
}

void	Commands::capCommand(const Server &srv, User *usr, std::vector<std::string> params)
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

void	Commands::nickCommand(const Server &srv, User *usr, std::vector<std::string> params)
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

void	Commands::userCommand(const Server &srv, User *usr, std::vector<std::string> params)
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

void	Commands::pingCommand(const Server &srv, User *usr, std::vector<std::string> params)
{
	(void) srv;
	(void) usr;
	(void) params;
	#ifdef DEBUG
		std::cout << ">> PING command executed" << std::endl;
	#endif
}

void	Commands::pongCommand(const Server &srv, User *usr, std::vector<std::string> params)
{
	(void) srv;
	if (params[0] != IP && params[0] != SRV_NAME)
		throw (Replies::ErrException(ERR_NOSUCHSERVER(usr->getNick(), usr->getUser(), params[0]).c_str()));
	#ifdef DEBUG
		std::cout << ">> PONG command executed" << std::endl;
	#endif
	usr->setWriteBuff(usr->getWriteBuff() + MSG_PONG(IP));
}
