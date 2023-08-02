/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 09:27:23 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/02 10:44:17 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

std::map<std::string, Commands::cmd_p >	Commands::commands;

void	Commands::initCommands(void)
{
	Commands::commands[PASS] = Commands::passCommand;
	Commands::commands[CAP] = Commands::capCommand;
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
	send(usr->getSockFd(), CAPMSG.c_str(), CAPMSG.size(), MSG_DONTWAIT);
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
}
