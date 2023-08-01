/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 08:59:58 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/01 16:14:09 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <map>
# include "irc.hpp"
# include "User.hpp"
# include "Server.hpp"

class	Server;
class	User;

class	Commands
{
	typedef void (*cmd_p)(const Server &, User *, std::vector<std::string>);

	private:
		// constructor and destructor are private 'cause this is a static class
		Commands(void) {};
		~Commands(void) {};

	public:

		// commands
		static void	capCommand(Server const &srv, User *usr, std::vector<std::string> params);
		static void	passCommand(Server const &srv, User *usr, std::vector<std::string> params);
		static void	nickCommand(Server const &srv, User *usr, std::vector<std::string> params);
		// functions
		static void	initCommands(void);
		// vaiables
		static std::map<std::string, cmd_p>	commands;
};

class	Replies
{
	private:
		// constructor and destructor are private 'cause this is a static class
		Replies(void) {};
		~Replies(void) {};

	public:
		// exceptions
		class	ErrException: public std::exception
		{
			private:
				const char *	_message;

			public:
				ErrException(const char * message): _message(message) {};
				virtual const char * what() const throw()
				{
					return (this->_message);
				}
		};
};

#endif
