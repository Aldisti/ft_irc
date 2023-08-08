/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 16:19:17 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/07 17:16:27 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc.hpp"

class Channel
{
	private:
		std::vector<User *>	_users;
		User				*_creator;
		std::string			_name;

	public:
		// CO functions
		Channel(User *creator);
		Channel(Channel const &c);
		~Channel(void);
		Channel	&operator=(Channel const &c);

		// getters
		User				*getCreator(void) const;
		std::vector<User *>	getUsers(void) const;
		std::string			getName(void) const;
		User				*getUser(int sfd) const;
		User				*getUser(std::string nick) const;

		// setters
		void	setCreator(User *creator);
		void	setName(std::string name);

		// functions
		void	addUser(User *usr);
		void	removeUser(int sfd);
		void	removeUser(std::string nick);
};
