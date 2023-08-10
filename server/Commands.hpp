/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 08:59:58 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/09 14:28:07 by gpanico          ###   ########.fr       */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "irc.hpp"

class	Server;
class	User;

class	Commands
{
	typedef void (*cmd_p)(Server &, User *, std::vector<std::string>);

	private:
		// constructor and destructor are private 'cause this is a static class
		Commands(void) {};
		~Commands(void) {};

		// functions
		static void	leaveAllChannels(Server &srv, User *usr);

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
		static void	noticeCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	privmsgCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	modeCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	wallopsCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	squitCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	killCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	awayCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	joinCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	partCommand(Server &srv, User *usr, std::vector<std::string> params);
		static void	serviceCommand(Server &srv, User *usr, std::vector<std::string> params);
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
