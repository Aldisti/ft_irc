/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 08:59:58 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/07 15:02:49 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <map>
# include "irc.hpp"
# include "utils.hpp"
# include "User.hpp"
# include "Server.hpp"

class	Server;
class	User;

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
		static void	userCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	pingCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	pongCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	errorCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	operCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	privmsgCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	modeCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	wallopsCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	squitCommand(Server &srv, User *usr, std::vector<std::string> params);
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
				std::string	*_message;

			public:
				ErrException(const char * message) {
					this->_message = ft_strdup(message); // ft_strdup
				}
				virtual	~ErrException() throw() {
					delete this->_message;
				}
				virtual const char * what() const throw() {
					return (this->_message->c_str());
				}
		};
};

#endif
