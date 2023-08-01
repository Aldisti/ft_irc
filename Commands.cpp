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

void	Commands::passCommand(Server *srv, User *usr, std::vector<std::string> params)
{
	if (params.size() == 0)
		throw (Replies::ErrException("")); // ErrNeedMoreParams
	if (usr->getReg() != 0)
		throw (Replies::ErrException("")); // ErrAlreadyRegistered
	if (params[0] != srv->getPass())
		throw (Replies::ErrException("")); // ErrIncorrectPassword
	usr->setReg(1);
}

void	Commands::capCommand(Server *srv, User *usr, std::vector<std::string> params)
{
	(void) usr;
	(void) params;
	srv.send(CAPMSG);
}

void	Commands::nickCommand(Server *srv, User *usr, std::vector<std::string> params)
{
	if (usr->getReg % 2 == 0)
		// throw (Replies::ErrNotRegistered()); // we can also use this reply
		throw (Replies::ErrException("")); // the command PASS has not yet been sent
	if (params.size() == 0)
		throw (Replies::ErrException("")); // nick not passed
	if (!User::checkNick(params[0]))
		throw (Replies::ErrException("")); // the nick passed contains unacceptable characters
	if (srv->getUser(paramas[0]))
		throw (Replies::ErrException("")); // nickname is already paired with another User
	usr->setNick(params[0]);
	usr->setReg(usr->getReg() | 2);
}
