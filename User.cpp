/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:20:48 by adi-stef          #+#    #+#             */
/*   Updated: 2023/06/30 16:26:13 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

bool	User::checkNick(std::string nick)
{
	if (nick.size() < 1 || nick.size() > 9)
		return (false);
	if (LETTERS.find(nick[0]) == NPOS && SPECIAL.find(nick[0]) == NPOS)
		return (false);
	for (int i = 1; i < (int) nick.size(); i++)
		if (LETTERS.find(nick[0]) == NPOS && SPECIAL.find(nick[0]) == NPOS
				&& DIGITS.find(nick[0]) == NPOS || nick[0] != '-')
			return (false);
	return (true);
}

User::User(int sockfd): _sockfd(sockfd), _registered(0), _op(false)
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
