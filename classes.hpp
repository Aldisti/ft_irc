/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   classes.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 10:24:01 by gpanico           #+#    #+#             */
/*   Updated: 2023/06/29 15:43:35 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLASSES_HPP
# define CLASSES_HPP

# include "irc.hpp"

# define LETTERS std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
# define DIGITS std::string("0123456789")
# define HEXDIGITS std::string("0123456789ABCDEF")
# define SPECIAL std::string("[\]^_`{|}")
# define NPOS std::string::npos

class User
{
	private:
		// variables
		int			_sockfd;
		int			_registered;
		int			_mode;
		std::string	_nick;
		std::string	_user;
		std::string	_real;

		// static functions
		static bool	checkNick(std::string nick);

	public:
		// CO functions
		User(int sockfd);
		User(User const &usr);
		~User(void);
		User		&operator=(User const &usr);

		// getters
		std::string	getNick(void) const;
		std::string	getUser(void) const;
		std::string	getReal(void) const;
		int			getSockFd(void) const;
		int			getReg(void) const;
		int			getMode(void) const;

		// setters
		void		setNick(std::string nick);
		void		setUser(std::string user);
		void		setReal(std::string real);
		void		setSockFd(int sfd);
		void		setReg(int reg);
		void		setMode(int mode);

};




#endif
