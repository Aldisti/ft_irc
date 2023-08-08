/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 10:24:01 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/08 09:03:16 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "irc.hpp"

class Server;

class User
{
	private:
		// variables
		int			_sockfd;
		int			_registered;
		int			_mode;
		bool		_op;
		bool		_close;
		bool		_ping;
		long		_time;
		std::string	_nick;
		std::string	_user;
		std::string	_real;
		std::string	_readBuff;
		std::string	_writeBuff;
		std::string	_awayMsg;

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
		bool			getClose(void) const;
		bool			getPing(void) const;
		long			getTime(void) const;
		std::string		getNick(void) const;
		std::string		getUser(void) const;
		std::string		getReal(void) const;
		std::string		getReadBuff(void) const;
		std::string		getWriteBuff(void) const;
		std::string		getAwayMsg(void) const;

		// setters
		void		setSockFd(int sfd);
		void		setReg(int reg);
		void		setMode(int mode);
		void		setOperator(bool op);
		void		setClose(bool close);
		void		setPing(bool ping);
		void		setNick(std::string nick);
		void		setUser(std::string user);
		void		setReal(std::string real);
		void		setReadBuff(std::string buff);
		void		setWriteBuff(std::string buff);
		void		setAwayMsg(std::string awayMsg);
		void		resetTime(void);

		// functions
		void		checkBuff(Server &server);

		// static functions
		static bool	checkNick(std::string nick);
};

#endif
