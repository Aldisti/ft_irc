/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 10:15:13 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/10 11:32:28 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "irc.hpp"

class User;
class Channel;

class Server
{
	private:
		// variables
		bool							_end;
		bool							_toClean;
		const std::string				_pass;
		std::vector<User *>				_users;
		std::vector<Channel *>			_channels;
		std::vector<struct sockaddr>	_theirAddr;
		socklen_t						_sinAddr;
		struct addrinfo					_hints;
		struct pollfd					_pollfds[BACKLOG];
		char							_buff[BUFFSIZE];
		int								_npollfds;
		int								_sfd;

		// functions
		void	checkClean(User *usr, int index);
		void	checkPing(User *usr);
		void	registerUser(void);
		void	checkFd(void);
		void	pollIn(User *usr, int index);
		void	pollOut(User *usr, int index);
		void	cleanPollfds(void);
		void	closeServer(void);
		void	closeConnection(User *usr, int index);

	public:
		// functions
		Server(std::string pass);
		~Server(void);
		void		setEvent(int fd, int event);
		void		addChannel(Channel *c);
		void		removeChannel(Channel *c);
		void		polling(void);
		
		// getters
		std::string				getPass(void) const;
		std::string				getIp(void) const;
		User					*getUser(int fd) const;
		User					*getUser(std::string nick) const;
		std::vector<User *>		getUsers() const;
		Channel					*getChannel(std::string name) const;
		std::vector<Channel *>	getChannels(void) const;

		// setters
		void				setEnd(bool end);
};
