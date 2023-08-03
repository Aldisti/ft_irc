/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 13:59:18 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/03 15:27:19 by adi-stef         ###   ########.fr       */
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
	#ifdef DEBUG
		std::cout << "##### SERVER CREATED SUCCESSFULLY #####" << std::endl;
	#endif
}

Server::~Server(void)
{
	close(this->_sfd);
	while (this->_users.size())
	{
		delete this->_users.back();
		this->_users.pop_back();
	}
	#ifdef DEBUG
		std::cout << "##### SERVER DELETED SUCCESSFULLY #####" << std::endl;
	#endif
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
	#ifdef DEBUG
		std::cout << "########## REGISTERING USER ##########" << std::endl;
	#endif
	this->_theirAddr.resize(this->_theirAddr.size() + 1);
	this->_pollfds[this->_npollfds].fd = accept(this->_sfd, &(this->_theirAddr.back()), &(this->_sinAddr));
	if (this->_pollfds[this->_npollfds].fd == -1)
	{
		this->_theirAddr.pop_back();
		#ifdef DEBUG
			std::cout << "########## REGISTRATION FAILED ##########" << std::endl;
		#endif
		throw (Server::ExceptionAccept());
	}
	this->_pollfds[this->_npollfds].events = POLLIN;
	this->_users.push_back(new User(this->_pollfds[this->_npollfds].fd));
	#ifdef DEBUG
		std::cout << ">> new user: index[" << this->_npollfds
		<< "] sfd [" << this->_pollfds[this->_npollfds].fd << "]" << std::endl;
	#endif
	this->_npollfds++;
	#ifdef DEBUG
		std::cout << "########## REGISTRATION COMPLETED ##########" << std::endl;
	#endif
}

void		Server::checkFd(void)
{
	User	*tmp;

	#ifdef DEBUG
		std::cout << "########## CHECKING FDs ##########" << std::endl;
	#endif
	for (int i = 1; i < this->_npollfds; i++)
	{
		if (this->_pollfds[i].revents == 0 || this->_pollfds[i].fd == -1)
			continue ;
		tmp = this->getUser(this->_pollfds[i].fd);
		#ifdef DEBUG
			std::cout << ">> checking user: index [" << i
			<< "] sfd [" << tmp->getSockFd()
			<< "] revents [" << this->_pollfds[i].revents << "]" << std::endl;
		#endif
		if (this->_pollfds[i].revents == POLLIN && this->_pollfds[i].fd != -1)
			this->pollIn(tmp, i);
		else if (this->_pollfds[i].revents == POLLOUT && this->_pollfds[i].fd != -1)
			this->pollOut(tmp, i);
		if (tmp->getWriteBuff() == "") {
			this->_pollfds[i].events = POLLIN;
			#ifdef DEBUG
				std::cout << ">> user events set to POLLIN [" << POLLIN << "]" << std::endl;
			#endif
		} else {
			this->_pollfds[i].events = POLLOUT;
			#ifdef DEBUG
				std::cout << ">> user events set to POLLOUT [" << POLLOUT << "]" << std::endl;
			#endif
		}
	}
	#ifdef DEBUG
		std::cout << "########## CHECKED FDs ##########" << std::endl;
	#endif
}

void	Server::pollIn(User *usr, int index)
{
	int			r;

	memset((void *) this->_buff, 0, BUFFSIZE); // ft_memset()
	r = recv(usr->getSockFd(), this->_buff, BUFFSIZE, MSG_DONTWAIT);
	#ifdef DEBUG
		std::cout << ">> received [" << r << "]:" << std::endl;
		std::cout << "["
		<< std::string(this->_buff).substr(0, std::string(this->_buff).size() - 2)
		<< "]" << std::endl;
	#endif
	if (r < 0)
	{
		std::cerr << "recv() failed" << std::endl;
		close(usr->getSockFd());
		usr->setSockFd(-1);
		this->_pollfds[index].fd = -1;
		this->_toClean = true;
		return ;
	}
	else if (r == 0)
	{
		std::cerr << "connection closed" << std::endl;
		close(usr->getSockFd());
		usr->setSockFd(-1);
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
		#ifdef DEBUG
			std::cout << ">> buffer checking failed" << std::endl;
		#endif
	}
	#ifdef DEBUG
		std::cout << ">> buffer checking completed" << std::endl;
	#endif
}

void	Server::pollOut(User *usr, int index)
{
	int			s;
	std::string	writeBuff;

	writeBuff = usr->getWriteBuff();
	#ifdef DEBUG
		std::cout << ">> writeBuff:\n["
		<< writeBuff.substr(0, writeBuff.size() - 2)
		<< "]" << std::endl;
	#endif
	s = send(usr->getSockFd(), writeBuff.c_str(), writeBuff.size(), MSG_DONTWAIT);
	#ifdef DEBUG
		std::cout << ">> sent [" << s << "]:" << std::endl;
		std::cout << "[" << writeBuff.substr(0, s - DEL.size()) << "]" << std::endl;
	#endif
	if (s < 0)
	{
		std::cerr << "send() failed" << std::endl;
		close(usr->getSockFd());
		usr->setSockFd(-1);
		this->_pollfds[index].fd = -1;
		this->_toClean = true;
		return ;
	}
	else if (s == 0)
	{
		std::cerr << "connection closed" << std::endl;
		close(usr->getSockFd());
		usr->setSockFd(-1);
		this->_pollfds[index].fd = -1;
		this->_toClean = true;
		return ;
	}
	else if (s < (int) writeBuff.size()) {
		usr->setWriteBuff(writeBuff.substr(s));
		#ifdef DEBUG
			std::cout << ">> writeBuff resized to ["
			<< (int) writeBuff.size() - s << "]:" << std::endl;
			std::cout << "["
			<< usr->getWriteBuff().substr(0, usr->getWriteBuff().size() - DEL.size())
			<< "]" << std::endl;
		#endif
	} else {
		usr->setWriteBuff("");
		#ifdef DEBUG
			std::cout << ">> writeBuff resetted" << std::endl;
		#endif
	}
}

void	Server::cleanPollfds(void) {
	int		size;
	User	*usr;

	size = this->_users.size();
	for (int i = 0; i < size; i++)
	{
		usr = this->_users[i];
		if (usr->getSockFd() == -1)
		{
			delete usr;
			this->_users.erase(this->_users.begin() + i);
			i--;
			size--;
		}
	}
	for (int i = 1; i < this->_npollfds - 1; i++)
	{
		if (this->_pollfds[i].fd == -1)
		{
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

void	Server::polling(void)
{
	int	rs;
	while (true)
	{
		std::cout << "polling..." << std::endl;
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
