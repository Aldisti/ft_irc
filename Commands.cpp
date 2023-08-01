/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 09:27:23 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/01 15:39:06 by gpanico          ###   ########.fr       */
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
		throw (Replies::ErrException(std::string("").c_str())); // ErrNeedMoreParams
	if (usr->getReg() != 0)
		throw (Replies::ErrException(std::string("").c_str())); // ErrAlreadyRegistered
	if (params[0] != srv.getPass())
		throw (Replies::ErrException(std::string("").c_str())); // ErrIncorrectPassword
	usr->setReg(1);
}

void	Commands::capCommand(const Server &srv, User *usr, std::vector<std::string> params)
{
	(void) usr;
	(void) params;
	(void) srv;
	//srv.send(CAPMSG);
}

void	Commands::nickCommand(const Server &srv, User *usr, std::vector<std::string> params)
{
	if (usr->getReg() % 2 == 0)
		// throw (Replies::ErrNotRegistered()); // we can also use this reply
		throw (Replies::ErrException(std::string("").c_str())); // the command PASS has not yet been sent
	if (params.size() == 0)
		throw (Replies::ErrException(std::string("").c_str())); // nick not passed
	if (!User::checkNick(params[0]))
		throw (Replies::ErrException(std::string("").c_str())); // the nick passed contains unacceptable characters
	if (srv.getUser(params[0]))
		throw (Replies::ErrException(std::string("").c_str())); // nickname is already paired with another User
	usr->setNick(params[0]);
	usr->setReg(usr->getReg() | 2);
}
