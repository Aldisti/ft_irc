/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 16:27:06 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/08 12:44:56 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"


Channel::Channel(User *creator, std::string name): _creator(creator), _name(name)
{
	this->_users.push_back(creator);
}

Channel::Channel(Channel const &c)
{
	*this = c;
}

Channel::~Channel(void)
{
	return ;
}

Channel	&Channel::operator=(Channel const &c)
{
	if (this == &c)
		return (*this);
	this->_users = c.getUsers();
	this->_creator = c.getCreator();
	return (*this);
}

User	*Channel::getCreator(void) const
{
	return (this->_creator);
}

std::vector<User *>	Channel::getUsers(void) const
{
	return (this->_users);
}

std::string	Channel::getName(void) const
{
	return (this->_name);
}

User	*Channel::getUser(int sfd) const
{
	for (int i = 0; i < (int) this->_users.size(); i++)
		if (this->_users[i]->getSockFd() == sfd)
			return (this->_users[i]);
	return (NULL);
}

User	*Channel::getUser(std::string nick) const
{
	for (int i = 0; i < (int) this->_users.size(); i++)
		if (this->_users[i]->getNick() == nick)
			return (this->_users[i]);
	return (NULL);
}

User	*Channel::getUser(User *usr) const
{
	for (int i = 0; i < (int) this->_users.size(); i++)
		if (this->_users[i] == usr)
			return (this->_users[i]);
	return (NULL);
}

std::string	Channel::getUserList(void) const
{
	std::string	list = "";

	for (int i = 0; i < (int) this->_users.size(); i++)
	{
		if (i)
			list += " ";
		list += C_NAME(this->_users[i]->getNick(), this->_users[i]->getUser());
	}
	return (list);
}

void	Channel::setCreator(User *creator)
{
	this->_creator = creator;
}

void	Channel::setName(std::string name)
{
	this->_name = name;
}

void	Channel::addUser(User *usr)
{
	this->_users.push_back(usr);
}

void	Channel::removeUser(int sfd)
{
	for (int i = 0; i < (int) this->_users.size(); i++)
		if (this->_users[i]->getSockFd() == sfd)
		{
			this->_users.erase(this->_users.begin() + i);
			break ;
		}
}

void	Channel::removeUser(std::string nick)
{
	for (int i = 0; i < (int) this->_users.size(); i++)
		if (this->_users[i]->getNick() == nick)
		{
			this->_users.erase(this->_users.begin() + i);
			break ;
		}
}
