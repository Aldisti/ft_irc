/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   classes.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 10:24:01 by gpanico           #+#    #+#             */
/*   Updated: 2023/06/30 08:58:57 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

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
		bool		_op;
		std::string	_nick;
		std::string	_user;
		std::string	_real;

	public:
		// CO functions
		User(int sockfd);
		User(User const &usr);
		~User(void);
		User		&operator=(User const &usr);

		// getters
		int			getSockFd(void) const;
		int			getReg(void) const;
		int			getMode(void) const;
		bool		getOperator(void) const;
		std::string	getNick(void) const;
		std::string	getUser(void) const;
		std::string	getReal(void) const;

		// setters
		void		setSockFd(int sfd);
		void		setReg(int reg);
		void		setMode(int mode);
		void		setOperator(bool op);
		void		setNick(std::string nick);
		void		setUser(std::string user);
		void		setReal(std::string real);

		// static functions
		static bool	checkNick(std::string nick);
};

#endif
