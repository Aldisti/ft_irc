/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 13:59:18 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/10 11:31:46 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::closeServer(void)
{
	close(this->_sfd);
	while (this->_users.size())
	{
		delete this->_users.back();
		this->_users.pop_back();
	}
	while (this->_channels.size())
	{
		delete this->_channels.back();
		this->_channels.pop_back();
	}
	MY_DEBUG("##### SERVER DELETED SUCCESSFULLY #####")
}

Server::Server(std::string pass): _end(false), _toClean(false), _pass(pass), _npollfds(1)
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
		throw (ErrException("getaddrinfo() failed"));
	this->_sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (this->_sfd == -1)
		throw (ErrException("socket() failed"));
	if (setsockopt(this->_sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)))
		throw (ErrException("setsockopt() failed"));
	if (bind(this->_sfd, res->ai_addr, res->ai_addrlen))
		throw (ErrException("bind() failed"));
	freeaddrinfo(res);
	if (listen(this->_sfd, BACKLOG))
		throw (ErrException("listen() faied"));
	memset((void *) this->_pollfds, 0, sizeof(this->_pollfds)); // ft_memset
	this->_pollfds[0].fd = this->_sfd;
	this->_pollfds[0].events = POLLIN;
	MY_DEBUG("##### SERVER CREATED SUCCESSFULLY #####")
}

Server::~Server(void)
{
	this->closeServer();
}

std::string	Server::getPass(void) const
{
	return (this->_pass);
}

User	*Server::getUser(int fd) const
{
	for (std::vector<User *>::const_iterator ite = this->_users.begin(); ite != this->_users.end(); ite++)
		if ((*ite)->getSockFd() == fd)
			return (*ite);
	return (NULL);
}

User	*Server::getUser(std::string nick) const
{
	for (std::vector<User *>::const_iterator ite = this->_users.begin(); ite != this->_users.end(); ite++)
		if ((*ite)->getNick() == nick)
			return (*ite);
	return (NULL);
}

std::vector<User *>	Server::getUsers(void) const
{
	return (this->_users);
}

Channel	*Server::getChannel(std::string name) const
{
	for (int i = 0; i < (int) this->_channels.size(); i++)
		if (this->_channels[i]->getName() == name)
			return (this->_channels[i]);
	return (NULL);
}

std::vector<Channel *>	Server::getChannels(void) const
{
	return (this->_channels);
}

void	Server::addChannel(Channel *c)
{
	this->_channels.push_back(c);
}

void	Server::removeChannel(Channel *c)
{
	for (int i = 0; i < (int) this->_channels.size(); i++)
	{
		if (c == this->_channels[i])
		{
			delete this->_channels[i];
			this->_channels.erase(this->_channels.begin() + i);
			break ;
		}
	}
}

void	Server::setEnd(bool end)
{
	this->_end = end;
}

void	Server::registerUser(void)
{
	char	*ip;

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
		throw (ErrException("accept() failed"));
	}
	this->_pollfds[this->_npollfds].events = POLLIN;
	ip = inet_ntoa((((struct sockaddr_in *) &(this->_theirAddr.back()))->sin_addr));
	this->_users.push_back(new User(this->_pollfds[this->_npollfds].fd, std::string(ip)));
	MY_DEBUG(">> new user: index[" << this->_npollfds << "] sfd [" << this->_pollfds[this->_npollfds].fd << "]")
	this->_npollfds++;
	MY_DEBUG("########## REGISTRATION COMPLETED ##########")
}

void		Server::closeConnection(User *usr, int index)
{
	MY_DEBUG("connection closed")
	usr->setClose(true);
	usr->setWriteBuff("");
	this->_pollfds[index].fd = -1;
	this->_toClean = true;
}

void		Server::checkClean(User *usr, int index)
{
	if (usr == NULL || index < 0 || index > this->_npollfds
		|| !usr->getClose() || usr->getWriteBuff() != "")
		return ;
	this->_toClean = true;
	this->_pollfds[index].fd = -1;
}

void		Server::checkPing(User *usr)
{
	if (!usr->getClose() && !usr->getPing() && ft_gettime() - usr->getTime() >= PING_TIMEOUT)
	{
		MY_DEBUG(">> sending PING message")
		usr->setWriteBuff(usr->getWriteBuff() + MSG_PING);
		this->setEvent(usr->getSockFd(), POLLOUT);
		usr->resetTime();
		usr->setPing(true);
	}
	else if (!usr->getClose() && usr->getPing() && ft_gettime() - usr->getTime() >= PING_TIMEOUT)
	{
		MY_DEBUG(">> PING_TIMEOUT elapsed")
		std::vector<std::string>	message(1, "Connection lost");
		Commands::errorCommand(*this, usr, message);
	}
}

void		Server::checkFd(void)
{
	User	*tmp;

	MY_DEBUG("########## CHECKING FDs ##########")
	for (int i = 1; i < this->_npollfds; i++)
	{
		tmp = this->getUser(this->_pollfds[i].fd);
		if (!tmp)
			continue ;
		MY_DEBUG(">> checking user: index [" << i << "] sfd [" << tmp->getSockFd() << "] revents [" << this->_pollfds[i].revents << "]")
		this->checkPing(tmp);
		if (this->_pollfds[i].revents != 0 && this->_pollfds[i].revents != POLLIN && this->_pollfds[i].revents != POLLOUT)
			this->closeConnection(tmp, i);
		else if (this->_pollfds[i].revents == 0 || this->_pollfds[i].fd == -1)
			continue ;
		MY_DEBUG((int) this->_buff[BUFFSIZE - 1])
		if (!tmp->getClose() && this->_pollfds[i].revents == POLLIN && this->_pollfds[i].fd != -1)
			this->pollIn(tmp, i);
		else if (this->_pollfds[i].revents == POLLOUT && this->_pollfds[i].fd != -1)
			this->pollOut(tmp, i);
		if (tmp->getWriteBuff() == "")
		{
			this->_pollfds[i].events = POLLIN;
			MY_DEBUG(">> user events set to POLLIN [" << POLLIN << "]")
		}
		else
		{
			this->_pollfds[i].events = POLLOUT;
			MY_DEBUG(">> user events set to POLLOUT [" << POLLOUT << "]")
		}
		this->checkClean(tmp, i);
	}
}

void	Server::pollIn(User *usr, int index)
{
	int			r;

	usr->resetTime();
	usr->setPing(false);
	memset((void *) this->_buff, 0, BUFFSIZE); // ft_memset()
	r = recv(usr->getSockFd(), this->_buff, BUFFSIZE, MSG_DONTWAIT);
	MY_DEBUG(">> received [" << r << "]:" << std::endl << "["
			<< std::string(this->_buff).substr(0, std::string(this->_buff).size() - 2) << "]")
	if (r < 0)
	{
		throw (ErrException("recv() failed"));
		this->closeConnection(usr, index);
		return ;
	}
	else if (r == 0)
	{
		this->closeConnection(usr, index);
		return ;
	}
	usr->setReadBuff(usr->getReadBuff() + std::string(this->_buff));
	try {
		usr->checkBuff(*this);
	} catch (ErrException &e) {
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
		throw (ErrException("send() failed"));
		this->closeConnection(usr, index);
		return ;
	}
	else if (s == 0)
	{
		this->closeConnection(usr, index);
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
	int					size;
	User				*usr;
	std::vector<User *>	users;

	size = this->_users.size();
	MY_DEBUG(">> cycling through users")
	for (int i = 0; i < size; i++)
	{
		usr = this->_users[i];
		if (usr->getClose() == true && usr->getWriteBuff() == "")
		{
			for (int j = 0; j < (int) this->_channels.size(); j++)
				this->_channels[j]->removeUser(usr->getSockFd());
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
	while (!this->_end || this->_users.size() != 0)
	{
		rs = poll(this->_pollfds, this->_npollfds, TIMEOUT);
		if (rs < 0)
		{
			close(this->_sfd);
			throw (ErrException("poll() failed"));
		}
		this->registerUser();
		this->checkFd();
		if (this->_toClean)
			this->cleanPollfds();
	}
}
