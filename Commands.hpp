/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 08:59:58 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/01 12:41:38 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <map>
# include "irc.hpp"
# include "User.hpp"
# include "Server.hpp"

class	Server;

class	Commands
{
	typedef void (*cmd_p)(Server &, User *, std::vector<std::string>);

	private:
		// constructor and destructor are private 'cause this is a static class
		Commands(void) {};
		~Commands(void) {};

	public:

		// commands
		static void	capCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	passCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	nickCommand(Server &srv, User *usr, std::vector<std::string> params);
		// functions
		void	initCommands(void);
		// vaiables
		static std::map<std::string, cmd_p>	_commands;
};

class	Replies
{
	private:
		// constructor and destructor are private 'cause this is a static class
		Replies(void) {};
		~Replies(void) {};

	public:
		// exceptions
		class	ErrNeedMoreParams: public std::exception
		{
			private:
				std::string	_message;

			public:
				ErrNeedMoreParams(std::string message): std::exception(), _message(message) {};
			virtual const char *what() const throw
			{
				return (this->_message.c_str());
			}
		};

		class	ErrAlreadyRegistered: public std::exception
		{
			private:
				std::string	_message;

			public:
				ErrAlreadyRegistered(std::string message): std::exception(), _message(message) {};
			virtual const char *what() const throw
			{
				return (this->_message.c_str());
			}
		};

		class	ErrIncorrectPassword: public std::exception
		{
			private:
				std::string	_message;

			public:
				ErrIncorrectPassword(std::string message): std::exception(), _message(message) {};
			virtual const char *what() const throw
			{
				return (this->_message.c_str());
			}
		};
};

#endif
