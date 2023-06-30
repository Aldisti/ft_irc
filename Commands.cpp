/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 09:27:23 by gpanico           #+#    #+#             */
/*   Updated: 2023/06/30 11:40:09 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

std::map<std::string, Commands::cmd_p >	Commands::_commands;

void	Commands::initCommands(void)
{
	Commands::_commands[PASS] = Commands::passCommand;
}

void	passCommand(Server *srv, User *usr, std::vector<std::string> params)
{
	if (params.size() == 0)
		throw (Replies::ErrNeedMoreParams());
	if (usr->getReg() != 0)
		throw (Replies::ErrAlreadyRegistered());
	if (params[0] != "pass")
		throw (Replies::ErrIncorrectPassword());
	usr->setReg(1);
}
