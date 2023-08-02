/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 08:27:25 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/02 14:14:35 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "User.hpp"

void	User::checkBuff(Server const &server)
{
	std::vector<std::string>	commands;
	std::vector<std::string>	cmd;
	std::string					cmdName;
	std::string					command;

//	std::cout << this->_buff.size() << std::endl;
//	std::cout << this->_buff << std::endl;
//	std::cout << "| ";
//	for (int i = 0; i < (int) this->_buff.size(); i++) {
//		std::cout << (int) this->_buff[i] << " ";
//	}
//	std::cout << "|" << std::endl;
	if (this->_buff.size() < MAX_BUFF && this->_buff.substr(this->_buff.size() - 2) != DEL)
		return ;
	if (this->_buff.size() > MAX_BUFF - 2)
	{
		this->_buff.resize(MAX_BUFF - 2);
		this->_buff += DEL;
	}
	commands = ft_split(this->_buff, DEL);
	for (int i = 0; i < (int) commands.size(); i++)
	{
		command = commands[i];
		try {
			cmd = ft_parse(command);
		} catch (std::exception &e) {
			this->_buff = "";
			throw Replies::ErrException(ERR_BADSYNTAX(this->_nick, this->_user).c_str());
		}
		if (command[0] == ':' && cmd[0] != this->_nick) {
			this->_buff = "";
			throw Replies::ErrException(ERR_NOSUCHNICK(this->_nick, this->_user).c_str());
		}
		else if (command[0] == ':')
			cmd.erase(cmd.begin());
		cmdName = cmd[0];
		cmd.erase(cmd.begin());
		try {
			Commands::commands.at(cmdName)(server, this, cmd);
		} catch (std::out_of_range &e) {
			std::cout << cmdName << std::endl;
			this->_buff = "";
			throw Replies::ErrException(ERR_UNKNOWNCOMMAND(this->_nick, this->_user, cmdName).c_str());
		}
	}
	this->_buff = "";
}

bool	User::checkNick(std::string nick)
{
	if (nick.size() < 1 || nick.size() > 9)
		return (false);
	if (LETTERS.find(nick[0]) == NPOS && SPECIAL.find(nick[0]) == NPOS)
		return (false);
	for (int i = 1; i < (int) nick.size(); i++)
		if (LETTERS.find(nick[i]) == NPOS && SPECIAL.find(nick[i]) == NPOS
				&& DIGITS.find(nick[i]) == NPOS && nick[i] != '-')
			return (false);
	return (true);
}

User::User(int sockfd): _sockfd(sockfd), _registered(0), _op(false), _buff("")
{
	return ;
}

User::User(User const &usr)
{
	*this = usr;
}

User::~User(void)
{
	close(this->_sockfd);
}

User		&User::operator=(User const &usr)
{
	if (this == &usr)
		return (*this);
	this->_sockfd = usr.getSockFd();
	this->_registered = usr.getReg();
	this->_mode = usr.getMode();
	this->_op = usr.getOperator();
	this->_nick = usr.getNick();
	this->_user = usr.getUser();
	this->_real = usr.getReal();
	return (*this);
}

int			User::getSockFd(void) const
{
	return (this->_sockfd);
}

int			User::getReg(void) const
{
	return (this->_registered);
}

int			User::getMode(void) const
{
	return (this->_mode);
}

bool		User::getOperator(void) const
{
	return (this->_op);
}

std::string	User::getNick(void) const
{
	return (this->_nick);
}

std::string	User::getUser(void) const
{
	return (this->_user);
}

std::string	User::getReal(void) const
{
	return (this->_real);
}

std::string	User::getBuff(void) const
{
	return (this->_buff);
}

void		User::setSockFd(int sfd)
{
	this->_sockfd = sfd;
}

void		User::setReg(int reg)
{
	this->_registered = reg;
}

void		User::setMode(int mode)
{
	this->_mode = mode;
}

void		User::setOperator(bool op)
{
	this->_op = op;
}

void		User::setNick(std::string nick)
{
	this->_nick = nick;
}

void		User::setUser(std::string user)
{
	this->_user = user;
}

void		User::setReal(std::string real)
{
	this->_real = real;
}

void		User::setBuff(std::string buff)
{
	this->_buff = buff;
}
