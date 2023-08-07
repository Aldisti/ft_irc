/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 10:15:13 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/07 15:06:06 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "User.hpp"
# include "Commands.hpp"

class User;

class Server
{
	private:
		// variables
		bool							_end;
		bool							_toClean;
		const std::string				_pass;
		std::vector<User *>				_users;
		std::vector<struct sockaddr>	_theirAddr;
		socklen_t						_sinAddr;
		struct addrinfo					_hints;
		struct pollfd					_pollfds[BACKLOG];
		char							_buff[BUFFSIZE];
		int								_npollfds;
		int								_sfd;
		// functions
		void	checkClean(User *usr, int index);
		bool	checkPing(User *usr, int i);

	public:
		// functions
		Server(std::string pass);
		~Server(void);
		void		registerUser(void);
		void		checkFd(void);
		void		polling(void);
		void		pollIn(User *usr, int index);
		void		pollOut(User *usr, int index);
		void		cleanPollfds(void);
		void		setEvent(int fd, int event);
		void		closeServer(void);
		
		// getters
		std::string			getPass(void) const;
		std::string			getIp(void) const;
		User				*getUser(int fd) const;
		User				*getUser(std::string nick) const;
		std::vector<User *>	getUsers() const;

		// setters
		void				setEnd(bool end);

	public:
		// exceptions
		class ExceptionGetAddressInfo: public std::exception
		{
			virtual const char *what() const throw()
			{
				return ("getaddrinfo() failed");
			}
		};

		class ExceptionSocket: public std::exception
		{
			virtual const char *what() const throw()
			{
				return ("socket() failed");
			}
		};

		class ExceptionSetSockOpt: public std::exception
		{
			virtual const char *what() const throw()
			{
				return ("setsockopt() failed");
			}
		};

		class ExceptionBind: public std::exception
		{
			virtual const char *what() const throw()
			{
				return ("bind() failed");
			}
		};

		class ExceptionListen: public std::exception
		{
			virtual const char *what() const throw()
			{
				return ("listen() failed");
			}
		};

		class ExceptionAccept: public std::exception
		{
			virtual const char *what() const throw()
			{
				return ("accept() failed");
			}
		};
};

#endif
