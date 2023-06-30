/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 08:59:58 by gpanico           #+#    #+#             */
/*   Updated: 2023/06/30 12:23:31 by gpanico          ###   ########.fr       */
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

#endif
