/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 09:27:23 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/02 15:58:11 by gpanico          ###   ########.fr       */
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
		throw (Replies::ErrException(std::string(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), PASS)).c_str()));
	if (usr->getReg() != 0)
		throw (Replies::ErrException(std::string(ERR_ALREADYREGISTERED(usr->getNick(), usr->getUser())).c_str()));
	if (params[0] != srv.getPass())
		throw (Replies::ErrException(std::string(ERR_INCORRECTPASS(usr->getNick(), usr->getUser())).c_str()));
	usr->setReg(1);
}

void	Commands::capCommand(const Server &srv, User *usr, std::vector<std::string> params)
{
	(void) usr;
	(void) params;
	(void) srv;
	if (params[0] == "LS")
		send(usr->getSockFd(), MSG_CAP.c_str(), MSG_CAP.size(), MSG_DONTWAIT);
}

void	Commands::nickCommand(const Server &srv, User *usr, std::vector<std::string> params)
{
	if (usr->getReg() % 2 == 0)
		throw (Replies::ErrException(std::string(ERR_NOTREGISTERED(usr->getNick(), usr->getUser())).c_str()));
	if (params.size() == 0)
		throw (Replies::ErrException(std::string(ERR_NONICKNAMEGIVEN(usr->getNick(), usr->getUser())).c_str()));
	if (!User::checkNick(params[0]))
		throw (Replies::ErrException(std::string(ERR_ERRONEUSNICKNAME(usr->getNick(), usr->getUser())).c_str()));
	if (srv.getUser(params[0]))
		throw (Replies::ErrException(std::string(ERR_NICKNAMEINUSE(usr->getNick(), usr->getUser())).c_str()));
	usr->setNick(params[0]);
	usr->setReg(usr->getReg() | 2);
	if (usr->getReg() >= 7) {
		send(usr->getSockFd(), RPL_WELCOME(usr->getNick(), usr->getUser(), SRV_NAME).c_str(),
				RPL_WELCOME(usr->getNick(), usr->getUser(), SRV_NAME).size(), MSG_DONTWAIT);
	}
}

void	Commands::userCommand(const Server &srv, User *usr, std::vector<std::string> params)
{
	(void) srv;
	if (usr->getReg() % 2 == 0)
		throw (Replies::ErrException(std::string(ERR_NOTREGISTERED(usr->getNick(), usr->getUser())).c_str()));
	if (params.size() < 4)
		throw (Replies::ErrException(std::string(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), PASS)).c_str()));
	if ((usr->getReg() >> 2) % 2)
		throw (Replies::ErrException(std::string(ERR_ALREADYREGISTERED(usr->getNick(), usr->getUser())).c_str()));
	usr->setUser(params[0]);
	usr->setMode(atoi(params[1].c_str())); // ft_atoi
	usr->setReal(params[3]);
	usr->setReg(usr->getReg() | 4);
	if (usr->getReg() >= 7) {
		send(usr->getSockFd(), RPL_WELCOME(usr->getNick(), usr->getUser(), SRV_NAME).c_str(),
				RPL_WELCOME(usr->getNick(), usr->getUser(), SRV_NAME).size(), MSG_DONTWAIT);
	}
}

void	Commands::pingCommand(const Server &srv, User *usr, std::vector<std::string> params)
{
 (void) srv;
 (void) usr;
 (void) params;
}

void	Commands::pongCommand(const Server &srv, User *usr, std::vector<std::string> params)
{
	(void) srv;
	if (params[0] != IP && params[0] != SRV_NAME)
		throw (Replies::ErrException(ERR_NOSUCHSERVER(usr->getNick(), usr->getUser(), params[0]).c_str()));
	int r = send(usr->getSockFd(), MSG_PONG(IP).c_str(), MSG_PONG(IP).size(), MSG_DONTWAIT);
	std::cout << r << std::endl;
}
