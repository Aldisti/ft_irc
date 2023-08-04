/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 13:59:18 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/04 15:16:51 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string pass): _toClean(false), _pass(pass), _npollfds(1)
{
	struct addrinfo	*res;
	int				on = 1;

	this->_sinAddr = sizeof(struct sockaddr);
	memset((void *) &this->_hints, 0, sizeof(struct addrinfo)); // ft_memset
	this->_hints.ai_family = AF_UNSPEC;
	this->_hints.ai_socktype = SOCK_STREAM;
	this->_hints.ai_protocol = 0;
	this->_hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(IP.c_str(), MYPORT, &this->_hints, &res))
		throw (Server::ExceptionGetAddressInfo());
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
	memset((void *) this->_pollfds, 0, sizeof(this->_pollfds)); // ft_memset
	this->_pollfds[0].fd = this->_sfd;
	this->_pollfds[0].events = POLLIN;
	MY_DEBUG("##### SERVER CREATED SUCCESSFULLY #####")
}

Server::~Server(void)
{
	close(this->_sfd);
	while (this->_users.size())
	{
		delete this->_users.back();
		this->_users.pop_back();
	}
	MY_DEBUG("##### SERVER DELETED SUCCESSFULLY #####")
}

std::string	Server::getPass(void) const
{
	return (this->_pass);
}

User		*Server::getUser(int fd) const
{
	for (std::vector<User *>::const_iterator ite = this->_users.begin(); ite != this->_users.end(); ite++)
		if ((*ite)->getSockFd() == fd)
			return (*ite);
	return (NULL);
}

User		*Server::getUser(std::string nick) const
{
	for (std::vector<User *>::const_iterator ite = this->_users.begin(); ite != this->_users.end(); ite++)
		if ((*ite)->getNick() == nick)
			return (*ite);
	return (NULL);
}

void		Server::registerUser(void)
{
	if (this->_pollfds[0].revents != POLLIN || this->_pollfds[0].fd == -1) {
		return ;
	}
	MY_DEBUG("########## REGISTERING USER ##########")
	this->_theirAddr.resize(this->_theirAddr.size() + 1);
	this->_pollfds[this->_npollfds].fd = accept(this->_sfd, &(this->_theirAddr.back()), &(this->_sinAddr));
	if (this->_pollfds[this->_npollfds].fd == -1)
	{
		this->_theirAddr.pop_back();
		MY_DEBUG("########## REGISTRATION FAILED ##########")
		throw (Server::ExceptionAccept());
	}
	this->_pollfds[this->_npollfds].events = POLLIN;
	this->_users.push_back(new User(this->_pollfds[this->_npollfds].fd));
	MY_DEBUG(">> new user: index[" << this->_npollfds << "] sfd [" << this->_pollfds[this->_npollfds].fd << "]")
	this->_npollfds++;
	MY_DEBUG("########## REGISTRATION COMPLETED ##########")
}

void		Server::checkFd(void)
{
	User	*tmp;

	MY_DEBUG("########## CHECKING FDs ##########")
	for (int i = 1; i < this->_npollfds; i++)
	{
		if (this->_pollfds[i].revents == 0 || this->_pollfds[i].fd == -1)
			continue ;
		tmp = this->getUser(this->_pollfds[i].fd);
		MY_DEBUG(">> checking user: index [" << i << "] sfd [" << tmp->getSockFd() << "] revents [" << this->_pollfds[i].revents << "]")
		MY_DEBUG((int) this->_buff[BUFFSIZE - 1])
		if (this->_pollfds[i].revents == POLLIN && this->_pollfds[i].fd != -1)
			this->pollIn(tmp, i);
		else if (this->_pollfds[i].revents == POLLOUT && this->_pollfds[i].fd != -1)
			this->pollOut(tmp, i);
		if (tmp->getWriteBuff() == "") {
			this->_pollfds[i].events = POLLIN;
			MY_DEBUG(">> user events set to POLLIN [" << POLLIN << "]")
		} else {
			this->_pollfds[i].events = POLLOUT;
			MY_DEBUG(">> user events set to POLLOUT [" << POLLOUT << "]")
		}
		if (tmp->getClose() && tmp->getWriteBuff() == "")
		{
			this->_toClean = true;
			this->_pollfds[i].fd = -1;
		}
	}
	MY_DEBUG("########## CHECKED FDs ##########")
}

void	Server::pollIn(User *usr, int index)
{
	int			r;

	memset((void *) this->_buff, 0, BUFFSIZE); // ft_memset()
	r = recv(usr->getSockFd(), this->_buff, BUFFSIZE, MSG_DONTWAIT);
	MY_DEBUG(">> received [" << r << "]:" << std::endl << "[" << std::string(this->_buff).substr(0, std::string(this->_buff).size() - 2)
			<< "]")
	if (r < 0)
	{
		std::cerr << "recv() failed" << std::endl;
		usr->setClose(true);
		usr->setWriteBuff("");
		this->_pollfds[index].fd = -1;
		this->_toClean = true;
		return ;
	}
	else if (r == 0)
	{
		std::cerr << "connection closed" << std::endl;
		usr->setClose(true);
		usr->setWriteBuff("");
		this->_pollfds[index].fd = -1;
		this->_toClean = true;
		return ;
	}
	usr->setReadBuff(usr->getReadBuff() + std::string(this->_buff));
	try {
		usr->checkBuff(*this);
	} catch (Replies::ErrException &e) {
		usr->setWriteBuff(usr->getWriteBuff() + e.what());
		usr->setReadBuff("");
		MY_DEBUG(">> buffer checking failed" << std::endl << usr->getWriteBuff())
	}
	MY_DEBUG(">> buffer checking completed")
}

void	Server::pollOut(User *usr, int index)
{
	int			s;
	std::string	writeBuff;

	writeBuff = usr->getWriteBuff();
	MY_DEBUG(">> writeBuff:\n[" << writeBuff.substr(0, writeBuff.size() - 2) << "]")
	s = send(usr->getSockFd(), writeBuff.c_str(), writeBuff.size(), MSG_DONTWAIT);
	MY_DEBUG(">> sent [" << s << "]:" << std::endl << "[" << writeBuff.substr(0, s - DEL.size()) << "]")
	if (s < 0)
	{
		std::cerr << "send() failed" << std::endl;
		usr->setClose(true);
		usr->setWriteBuff("");
		this->_pollfds[index].fd = -1;
		this->_toClean = true;
		return ;
	}
	else if (s == 0)
	{
		std::cerr << "connection closed" << std::endl;
		usr->setClose(true);
		usr->setWriteBuff("");
		this->_pollfds[index].fd = -1;
		this->_toClean = true;
		return ;
	}
	else if (s < (int) writeBuff.size()) {
		usr->setWriteBuff(writeBuff.substr(s));
		MY_DEBUG(">> writeBuff resized to [" << (int) writeBuff.size() - s << "]:" << std::endl << "["
			<< usr->getWriteBuff().substr(0, usr->getWriteBuff().size() - DEL.size()) << "]")
	} else {
		usr->setWriteBuff("");
		MY_DEBUG(">> writeBuff resetted")
	}
}

void	Server::cleanPollfds(void) {
	int		size;
	User	*usr;

	size = this->_users.size();
  MY_DEBUG(">> cycling through users")
	for (int i = 0; i < size; i++)
	{
		usr = this->_users[i];
		if (usr->getClose() == true && usr->getWriteBuff() == "")
		{
      MY_DEBUG(">> deleting user id [" << (i + 1) << "] sfd [" << usr->getSockFd() << "]")
			delete usr;
			this->_users.erase(this->_users.begin() + i);
			i--;
			size--;
		}
	}
  MY_DEBUG(">> cycling through pollfds")
	for (int i = 1; i < this->_npollfds - 1; i++)
	{
		if (this->_pollfds[i].fd == -1)
		{
      MY_DEBUG(">> deleting user id [" << (i + 1) << "]")
			for (int j = i; j < this->_npollfds - 1; j++)
				this->_pollfds[j] = this->_pollfds[j + 1];
			i--;
			this->_npollfds--;
		}
	}
	if (this->_pollfds[this->_npollfds - 1].fd == -1)
		memset((void *) &this->_pollfds[--this->_npollfds], 0, sizeof(struct pollfd)); // ft_memset
	this->_toClean = false;
}

void	Server::setEvent(int fd, int event)
{
	if (event != POLLIN && event != POLLOUT)
		return ;
	for (int i = 1; i < this->_npollfds; i++)
		if (this->_pollfds[i].fd == fd)
		{
			this->_pollfds[i].events = event;
			break ;
		}
}

void	Server::polling(void)
{
	int	rs;
	while (true)
	{
		rs = poll(this->_pollfds, this->_npollfds, TIMEOUT);
		if (rs < 0)
		{
			std::cerr << "poll() failed" << std::endl; // throw something
			close(this->_sfd);
			return ;
		}
		this->registerUser();
		this->checkFd();
		if (this->_toClean)
			this->cleanPollfds();
	}
}
