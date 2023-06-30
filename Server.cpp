/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 13:59:18 by gpanico           #+#    #+#             */
/*   Updated: 2023/06/30 16:36:26 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string pass): _pass(pass), _npollfds(1)
{
	struct addrinfo	*res;
	int				on = 1;

	this->_sinAddr = sizeof(struct sockaddr);
	memset((void *) &this->_hints, 0, sizeof(struct addrinfo));
	this->_hints.ai_family = AF_UNSPEC;
	this->_hints.ai_socktype = SOCK_STREAM;
	this->_hints.ai_protocol = 0;
	this->_hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, "8000", &this->_hints, &res))
		throw (Server::ExceptionGetAddrInfo());
	this->_sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (this->_sfd == -1)
		throw (Server::ExceptionSocket());
	if (setsockopt(this->_sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)))
		throw (Server::ExceptionSetSockOpt());
	if (bind(this->_sfd, res->ai_addr, res->ai_addrlen))
		throw (Server::ExceptionBind());
	freeaddrinfo(res);
	if (listen(this->_sfd, BACKLOG))
		throw (Server::ExceptionListen());
	memset((void *) this->_pollfds, 0, sizeof(this->_pollfds));
	this->_pollfds[0].fd = this->_sfd;
	this->_pollfds[0].events = POLLIN;
}

Server::~Server(void)
{
	close(this->_sfd);
	while (this->_users.size())
	{
		delete this->_users.back();
		this->_users.pop_back();
	}
}

std::string	Server::getPass(void) const
{
	return (this->_pass);
}

User		*Server::getUser(int fd) const
{
	for (std::vector<User*>::iterator ite = this->_users.begin(); ite != this->_users.end(); ite++)
		if ((*ite)->getSockFd() == fd)
			return (*ite);
	return (NULL);
}

User		*Server::getUser(std::string nick) const
{
	for (std::vector<User *>::iterator ite = this->_users.begin(); ite != this->_users.end(); ite++)
		if ((*ite)->getNick() == nick)
			return (*ite);
	return (NULL);
}

void		Server::registerUser(void)
{
	if (this->_pollfds[0].revents != POLLIN || this->_pollfds[0].fd == -1)
		return ;
	this->_theirAddr.resize(this->_theirAddr.size() + 1);
	this->_pollfds[this->_npollfds].fd = accept(this->_sfd, &(this->_theirAddr.back()), &(this->_sinAddr));
	if (this->_pollfds[this->_npollfds].fd == -1)
	{
		this->_theirAddr.pop_back();
		throw (Server::ExceptionAccept());
	}
	this->_pollfds[this->_npollfds].events = POLLIN;
	this->_npollfds++;
}

void		Server::checkFd(int	rs)
{
	int	r;

	for (int i = 1; i < this->_npollfds && rs > 0; i++)
	{
		if (this->_pollfds[i].revents != POLLIN || this->_pollfds[i].fd == -1)
			continue ;
		memset((void *) this->_buff, 0, BUFFSIZE); // ft_memset()
		r = recv(this->_pollfds[i].fd, this->_buff, BUFFSIZE, MSG_DONTWAIT);
		if (r < 0)
		{
			std::cerr << "recv() failed" << std::endl;
			close(pollfds[i].fd);
			pollfds[i].fd = -1;
			continue ;
		}
		else if (r == 0)
		{
			std::cerr << "connection closed" << std::endl;
			close(pollfds[i].fd);
			pollfds[i].fd = -1;
			continue ;
		}
	}
}
