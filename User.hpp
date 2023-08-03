/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 10:24:01 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/03 09:59:12 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "irc.hpp"
# include "Server.hpp"
# include "Commands.hpp"
# include "utils.hpp"

class Server;

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
		std::string	_readBuff;
		std::string	_writeBuff;

	public:
		// CO functions
		User(int sockfd);
		User(User const &usr);
		~User(void);
		User		&operator=(User const &usr);

		// getters
		int				getSockFd(void) const;
		int				getReg(void) const;
		int				getMode(void) const;
		bool			getOperator(void) const;
		std::string		getNick(void) const;
		std::string		getUser(void) const;
		std::string		getReal(void) const;
		std::string		getReadBuff(void) const;
		std::string		getWriteBuff(void) const;

		// setters
		void		setSockFd(int sfd);
		void		setReg(int reg);
		void		setMode(int mode);
		void		setOperator(bool op);
		void		setNick(std::string nick);
		void		setUser(std::string user);
		void		setReal(std::string real);
		void		setReadBuff(std::string buff);
		void		setWriteBuff(std::string buff);

		// functions
		void		checkBuff(Server const &server);

		// static functions
		static bool	checkNick(std::string nick);
};

#endif
