/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 10:15:13 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/01 16:17:26 by gpanico          ###   ########.fr       */
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
		const std::string				_pass;
		std::vector<User *>				_users;
		std::vector<struct sockaddr>	_theirAddr;
		socklen_t						_sinAddr;
		struct addrinfo					_hints;
		struct pollfd					_pollfds[BACKLOG];
		char							_buff[BUFFSIZE];
		int								_npollfds;
		int								_sfd;


	public:
		// functions
		Server(std::string pass);
		~Server(void);
		void		registerUser(void);
		//void		parseCommand(User *usr );
		void		checkFd(int	rs);
		
		// getters
		std::string	getPass(void) const;
		User		*getUser(int fd) const;
		User		*getUser(std::string nick) const;

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
