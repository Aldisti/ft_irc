/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 09:27:23 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/10 15:15:26 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

std::map<std::string, Commands::cmd_p >	Commands::commands;

void	Commands::initCommands(void)
{
	Commands::commands[CAP] = Commands::capCommand;
	Commands::commands[PASS] = Commands::passCommand;
	Commands::commands[NICK] = Commands::nickCommand;
	Commands::commands[USER] = Commands::userCommand;
	Commands::commands[PING] = Commands::pongCommand;
	Commands::commands[PONG] = Commands::pingCommand;
	Commands::commands[QUIT] = Commands::errorCommand;
	Commands::commands[OPER] = Commands::operCommand;
	Commands::commands[MODE] = Commands::modeCommand;
	Commands::commands[PRIVMSG] = Commands::privmsgCommand;
	Commands::commands[NOTICE] = Commands::noticeCommand;
	Commands::commands[WALLOPS] = Commands::wallopsCommand;
	Commands::commands[SQUIT] = Commands::squitCommand;
	Commands::commands[KILL] = Commands::killCommand;
	Commands::commands[AWAY] = Commands::awayCommand;
	Commands::commands[JOIN] = Commands::joinCommand;
	Commands::commands[PART] = Commands::partCommand;
	Commands::commands[SERVICE] = Commands::serviceCommand;
}

void	Commands::passCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	if (params.size() == 0)
		throw (ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), PASS).c_str()));
	if (usr->getReg() != 0)
		throw (ErrException(ERR_ALREADYREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params[0] != srv.getPass())
		throw (ErrException(ERR_PASSWDMISMATCH(usr->getNick(), usr->getUser()).c_str()));
	MY_DEBUG(">> PASS command executed\nuser->sfd [" << usr->getSockFd() << "]")
	usr->setReg(1);
}

void	Commands::capCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	(void) usr;
	(void) params;
	(void) srv;
	if (params[0] == "LS")
		usr->setWriteBuff(usr->getWriteBuff() + MSG_CAP);
	MY_DEBUG(">> CAP command executed")
}

void	Commands::nickCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	if (usr->getReg() % 2 == 0)
		throw (ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() == 0)
		throw (ErrException(ERR_NONICKNAMEGIVEN(usr->getNick(), usr->getUser()).c_str()));
	if (!User::checkNick(params[0]))
		throw (ErrException(ERR_ERRONEUSNICKNAME(usr->getNick(), usr->getUser()).c_str()));
	if (srv.getUser(params[0]))
		throw (ErrException(ERR_NICKNAMEINUSE(usr->getNick(), usr->getUser()).c_str()));
	usr->setNick(ft_tolower(params[0]));
	usr->setReg(usr->getReg() | 2);
	MY_DEBUG(">> NICK command executed" << std::endl << "usr->sfd [" << usr->getSockFd() << "] " 
			<< "usr->nick [" << usr->getNick() << "] ")
	if (usr->getReg() == 7) {
		usr->setWriteBuff(usr->getWriteBuff() + RPL_WELCOME(usr->getNick(), usr->getUser(), SRV_NAME, usr->getIp()));
		usr->setWriteBuff(usr->getWriteBuff() + RPL_YOURHOST(usr->getNick()));
		MY_DEBUG(">> USER registered correctly from NICK command")
	}
}

void	Commands::userCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	(void) srv;
	if (usr->getReg() % 2 == 0)
		throw (ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 4)
		throw (ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), PASS).c_str()));
	if ((usr->getReg() >> 2) % 2)
		throw (ErrException(ERR_ALREADYREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	usr->setUser(ft_tolower(params[0]));
	usr->setMode(atoi(params[1].c_str())); // ft_atoi
	usr->setReal(params[3]);
	usr->setReg(usr->getReg() | 4);
	MY_DEBUG(">> USER command executed" << std::endl << "usr->sfd [" << usr->getSockFd() << "] "
		<< "usr->user [" << usr->getUser() << "] " << "usr->real [" << usr->getReal() << "] ")
	if (usr->getReg() == 7) {
		usr->setWriteBuff(usr->getWriteBuff() + RPL_WELCOME(usr->getNick(), usr->getUser(), SRV_NAME, usr->getIp()));
		usr->setWriteBuff(usr->getWriteBuff() + RPL_YOURHOST(usr->getNick()));
		MY_DEBUG(">> USER registered correctly from USER command")
	}
}

void	Commands::pingCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	(void) srv;
	(void) params;
	MY_DEBUG(">> PING command executed")
	usr->resetTime();
	usr->setPing(false);
}

void	Commands::pongCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	(void) srv;
	if (params[0] != usr->getNick() && params[0] != IP)
		throw (ErrException(ERR_NOSUCHSERVER(usr->getNick(), usr->getUser(), params[0]).c_str()));
	MY_DEBUG(">> PONG command executed")
	usr->setWriteBuff(usr->getWriteBuff() + MSG_PONG(IP));
}

void	Commands::errorCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	std::string	message;

	Commands::leaveAllChannels(srv, usr);
	message = params.size() == 0 ? std::string("") : params[0];
	usr->setClose(true);
	usr->setWriteBuff(usr->getWriteBuff() + MSG_ERROR(usr->getNick(), message));
	srv.setEvent(usr->getSockFd(), POLLOUT);
}

void	Commands::operCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	(void) srv;
	if (usr->getReg() < 7)
		throw (ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 2)
		throw (ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), OPER).c_str()));
	if (params[1] != OPER_PASSWORD)
		throw (ErrException(ERR_PASSWDMISMATCH(usr->getNick(), usr->getUser()).c_str()));
	MY_DEBUG(">> OPER command executed" << std::endl << ">> usr: sfd [" << usr->getSockFd() << "] is now an operator"
			<< std::endl << ">> usr user: old [" << usr->getUser() << "] new [" << params[1] << "]")
	usr->setUser(params[0]);
	usr->setOperator(true);
	usr->setMode(usr->getMode() | OPERATOR);
	usr->setWriteBuff(usr->getWriteBuff() + RPL_YOUREOPER(usr->getNick(), usr->getUser()));
}


void	Commands::noticeCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	User				*tmp;
	Channel				*chn;
	std::vector<User *>	usrVec;

	if (usr->getReg() < 7)
		throw (ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 1)
		throw (ErrException(ERR_NORECIPIENT(usr->getNick(), usr->getUser(), PRIVMSG).c_str()));
	if (params.size() < 2)
		throw (ErrException(ERR_NOTEXTTOSEND(usr->getNick(), usr->getUser()).c_str()));
	if (params[0].find('.') != NPOS)
		throw (ErrException(ERR_BADMASK(usr->getNick(), usr->getUser(), params[0]).c_str()));
	if (CHANNEL.find(params[0][0]) == NPOS)
	{
		MY_DEBUG(">> trying to find user with nick: " + params[0])
		if ((tmp = srv.getUser(ft_tolower(params[0]))) == NULL)
			throw (ErrException(ERR_NOSUCHNICK(usr->getNick(), usr->getUser(), params[0]).c_str()));
		MY_DEBUG(">> user found with nick: " << tmp->getNick())
		if (tmp->getMode() & F_AWAY)
		{
			usr->setWriteBuff(usr->getWriteBuff() + RPL_AWAY(usr->getNick(), usr->getUser(), tmp->getNick(), tmp->getAwayMsg()).c_str());
			return ;
		}
		tmp->setWriteBuff(tmp->getWriteBuff() + MSG_NOTICE(usr->getNick(), usr->getUser(), tmp->getNick(), params[1]));
		srv.setEvent(tmp->getSockFd(), POLLOUT);
	}
	else
	{
		chn = srv.getChannel(ft_tolower(params[0]));
		if (chn == NULL)
	 		throw (ErrException(ERR_NOSUCHCHANNEL(usr->getNick(), usr->getUser(), chn->getName()).c_str()));
		if (chn->getUser(usr->getNick()) == NULL)
	 		throw (ErrException(ERR_CANNOTSENDTOCHAN(usr->getNick(), usr->getUser(), chn->getName()).c_str()));
		usrVec = chn->getUsers();
		for (int i = 0; i < (int) usrVec.size(); i++)
		{
			tmp = usrVec[i];
			if (tmp == usr)
				continue ;
			tmp->setWriteBuff(tmp->getWriteBuff() + MSG_NOTICE(usr->getNick(), usr->getUser(), chn->getName(), params[1]));
			srv.setEvent(tmp->getSockFd(), POLLOUT);
		}
	}
}

void	Commands::privmsgCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	User				*tmp;
	Channel				*chn;
	std::vector<User *>	usrVec;

	if (usr->getReg() < 7)
		throw (ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 1)
		throw (ErrException(ERR_NORECIPIENT(usr->getNick(), usr->getUser(), PRIVMSG).c_str()));
	if (params.size() < 2)
		throw (ErrException(ERR_NOTEXTTOSEND(usr->getNick(), usr->getUser()).c_str()));
	if (params[0].find('.') != NPOS)
		throw (ErrException(ERR_BADMASK(usr->getNick(), usr->getUser(), params[0]).c_str()));
	if (CHANNEL.find(params[0][0]) == NPOS)
	{
		MY_DEBUG(">> trying to find user with nick: " + params[0])
		if ((tmp = srv.getUser(ft_tolower(params[0]))) == NULL)
			throw (ErrException(ERR_NOSUCHNICK(usr->getNick(), usr->getUser(), params[0]).c_str()));
		MY_DEBUG(">> user found with nick: " << tmp->getNick())
		if (tmp->getMode() & F_AWAY)
		{
			usr->setWriteBuff(usr->getWriteBuff() + RPL_AWAY(usr->getNick(), usr->getUser(), tmp->getNick(), tmp->getAwayMsg()).c_str());
			return ;
		}
		tmp->setWriteBuff(tmp->getWriteBuff() + MSG_PRIVMSG(usr->getNick(), usr->getUser(), tmp->getNick(), params[1]));
		srv.setEvent(tmp->getSockFd(), POLLOUT);
	}
	else
	{
		chn = srv.getChannel(ft_tolower(params[0]));
		if (chn == NULL)
	 		throw (ErrException(ERR_NOSUCHCHANNEL(usr->getNick(), usr->getUser(), chn->getName()).c_str()));
		if (chn->getUser(usr->getNick()) == NULL)
	 		throw (ErrException(ERR_CANNOTSENDTOCHAN(usr->getNick(), usr->getUser(), chn->getName()).c_str()));
		usrVec = chn->getUsers();
		for (int i = 0; i < (int) usrVec.size(); i++)
		{
			tmp = usrVec[i];
			if (tmp == usr)
				continue ;
			tmp->setWriteBuff(tmp->getWriteBuff() + MSG_PRIVMSG(usr->getNick(), usr->getUser(), chn->getName(), params[1]));
			srv.setEvent(tmp->getSockFd(), POLLOUT);
		}
	}
}

void	Commands::modeCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	char		first;
	std::string	modes = "";

	(void) srv;
	if (usr->getReg() < 7)
		throw (ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 1)
		throw (ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), MODE).c_str()));
	if (CHANNEL.find(params[0][0]) != NPOS)
		return ;
	if (usr->getNick() != ft_tolower(params[0]))
		throw (ErrException(ERR_USERSDONTMATCH(usr->getNick(), usr->getUser()).c_str()));
	for (int i = 1; i < (int) params.size(); i++)
	{
		first = params[i][0];
		if (first == '+' || first == '-') {
			modes.append(1, params[i][0]);
			for (int j = 1; j < (int) params[i].size(); j++) {
				if (VALID_MODES.find(params[i][j]) == NPOS)
					throw (ErrException(ERR_UMODEUNKNOWNFLAG(usr->getNick(), usr->getUser()).c_str()));
				if (first == '+') {
					if (params[i][j] == 'o' || params[i][j] == 'O')
						continue ;
					usr->setMode(usr->getMode() | ft_convertToMode(params[i][j]));
				}
				else {
					if (params[i][j] == 'o' || params[i][j] == 'O')
						usr->setOperator(false);
					usr->setMode(usr->getMode() & (~ft_convertToMode(params[i][j])));
				}
				modes.append(1, params[i][j]);
			}
			modes.append(1, ' ');
		} else {
			throw (ErrException(ERR_UMODEUNKNOWNFLAG(usr->getNick(), usr->getUser()).c_str()));
		}
	}
	usr->setWriteBuff(usr->getWriteBuff() + RPL_UMODEIS(usr->getNick(), usr->getUser(), modes));
}

void	Commands::wallopsCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	std::vector<User *>	tmpUsers;

	if (usr->getReg() < 7)
		throw (ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 1)
		throw (ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), WALLOPS).c_str()));
	tmpUsers = srv.getUsers();
	for (int i = 0; i < (int) tmpUsers.size(); i++)
	{
		if (tmpUsers[i]->getNick() == usr->getNick() || !(tmpUsers[i]->getMode() & WALLOP))
			continue ;
		tmpUsers[i]->setWriteBuff(tmpUsers[i]->getWriteBuff() + PREFIX(usr->getNick(),
					usr->getUser()) + " " + usr->getNick() + " " + WALLOPS + " :" + params[0] + DEL);
		srv.setEvent(tmpUsers[i]->getSockFd(), POLLOUT);
	}
}

void	Commands::squitCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	std::vector<User *>	tmpUsers;
	MY_DEBUG("prova")
	if (params.size() < 2)
		throw (ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), SQUIT).c_str()));
	if (params[0] != IP && params[0] != SRV_NAME)
		throw (ErrException(ERR_NOSUCHSERVER(usr->getNick(), usr->getUser(), params[0]).c_str()));
	if (!usr->getOperator())
		throw (ErrException(ERR_NOPRIVILEGES(usr->getNick(), usr->getUser()).c_str()));
	std::vector<std::string>	tmp(params.begin() + 1, params.end());
	tmpUsers = srv.getUsers();
	for (int i = 0; i < (int) tmpUsers.size(); i++)
	{
		tmpUsers[i]->setClose(true);
		tmpUsers[i]->setWriteBuff("");
	}
	Commands::wallopsCommand(srv, usr, tmp);
	srv.setEnd(true);
}

void	Commands::killCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	User						*tmp = NULL;
	std::vector<std::string>	*partParams;
	std::vector<Channel *>		channels;

	if (usr->getReg() < 7)
		throw (ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (!usr->getOperator())
		throw (ErrException(ERR_NOPRIVILEGES(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 2)
		throw (ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), KILL).c_str()));
	if ((tmp = srv.getUser(ft_tolower(params[0]))) == NULL)
		throw (ErrException(ERR_NOSUCHNICK(usr->getNick(), usr->getUser(), params[0]).c_str()));
	MY_DEBUG(">> killing user with nick: " << tmp->getNick())
	channels = srv.getChannels();
	for (int i = 0; i < (int) channels.size(); i++)
		try
		{
			partParams = new std::vector<std::string>();
			partParams->push_back(channels[i]->getName());
			Commands::partCommand(srv, tmp, *partParams);
			delete partParams;
		}
		catch (ErrException &e)
		{
			delete partParams;
		}
	tmp->setClose(true);
	usr->setWriteBuff(usr->getWriteBuff() + MSG_KILL(usr->getNick(), tmp->getNick(), params[1]));
	tmp->setWriteBuff(tmp->getWriteBuff() + MSG_KILL(usr->getNick(), tmp->getNick(), params[1]));
	srv.setEvent(tmp->getSockFd(), POLLOUT);
}

void	Commands::awayCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	(void) srv;

	if (usr->getReg() < 7)
		throw (ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() == 0)
	{
		usr->setAwayMsg(MSG_AWAY);
		usr->setMode(usr->getMode() & (~F_AWAY));
		usr->setWriteBuff(usr->getWriteBuff() + RPL_UNAWAY(usr->getNick(), usr->getUser()).c_str());
	}
	else
	{
		usr->setAwayMsg(params[0]);
		usr->setMode(usr->getMode() | F_AWAY);
		usr->setWriteBuff(usr->getWriteBuff() + RPL_NOWAWAY(usr->getNick(), usr->getUser()).c_str());
	}
}

void	Commands::leaveAllChannels(Server &srv, User *usr)
{
	std::vector<Channel *>		channels;
	std::vector<std::string>	name(1, "");

	MY_DEBUG(">> exiting all the channels")
	channels = srv.getChannels();
	for (int i = 0; i < (int) channels.size(); i++)
	{
		if (channels[i]->getUser(usr) == NULL)
			continue ;
		if (i)
			name[0] += ",";
		name[0] += channels[i]->getName();
	}
	Commands::partCommand(srv, usr, name);
}

void	Commands::joinCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	Channel						*tmp;
	std::vector<std::string>	channelNames;
	std::vector<User *>			users;

  	if (usr->getReg() < 7)
  		throw (ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
  	if (params.size() < 1)
  		throw (ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), JOIN).c_str()));
  	if (params[0] == "0")
  	{
  		Commands::leaveAllChannels(srv, usr);
  		return ;
  	}
  	channelNames = ft_split(ft_tolower(params[0]), ",");
  	for (int i = 0; i < (int) channelNames.size(); i++)
  	{
		if (CHANNEL.find(channelNames[i][0]) == NPOS || channelNames[i].size() > 51)
			throw (ErrException(ERR_NOSUCHCHANNEL(usr->getNick(), usr->getUser(), channelNames[i]).c_str()));
		for (int j = 0; j < (int) NOT_CHANSTRING.size(); j++)
			if (channelNames[i].find(NOT_CHANSTRING[j]) != NPOS)
				throw (ErrException(ERR_NOSUCHCHANNEL(usr->getNick(), usr->getUser(), channelNames[i]).c_str()));
  		if ((tmp = srv.getChannel(channelNames[i])) == NULL)
  		{
  			tmp = new Channel(usr, channelNames[i]);
  			srv.addChannel(tmp);
  			MY_DEBUG(">> adding new channel " << channelNames[i])
  		}
  		else if (tmp->getUser(usr) == NULL)
  		{
  			tmp->addUser(usr);
  			MY_DEBUG(">> channel joined " << channelNames[i])
  		}
  		else
  			throw (ErrException(ERR_USERONCHANNEL(usr->getNick(), usr->getUser(), channelNames[i]).c_str()));
		users = tmp->getUsers();
  		usr->setWriteBuff(usr->getWriteBuff() + MSG_JOIN(usr->getNick(), usr->getUser(), channelNames[i]));
  		usr->setWriteBuff(usr->getWriteBuff() + RPL_NOTOPIC(usr->getNick(), usr->getUser(), channelNames[i]));
		for (int j = 0; j < (int) users.size(); j++)
		{
  			users[j]->setWriteBuff(users[j]->getWriteBuff() + RPL_NAMREPLY(users[j]->getNick(), users[j]->getUser(),
						channelNames[i], tmp->getUserList()));
  			users[j]->setWriteBuff(users[j]->getWriteBuff() + RPL_ENDOFNAMES(users[j]->getNick(), users[j]->getUser(), channelNames[i]));
			srv.setEvent(users[j]->getSockFd(), POLLOUT);
		}
  	}
}

void	Commands::partCommand(Server &srv, User *usr, std::vector<std::string> params)
{
	std::vector<std::string>	chnNames;
	User						*tmp;
	Channel						*chn;
	std::vector<User *>			usrVec;
	std::string					partMessage;

	if (usr->getReg() < 7)
		throw (ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 1)
		throw (ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), KILL).c_str()));
	chnNames = ft_split(ft_tolower(params[0]), std::string(","));
	partMessage = params.size() == 1 ? usr->getNick() : params[1];
	for (int i = 0; i < (int) chnNames.size(); i++)
	{
		chn = srv.getChannel(chnNames[i]);
		if (chn == NULL)
	 		throw (ErrException(ERR_NOSUCHCHANNEL(usr->getNick(), usr->getUser(), chn->getName()).c_str()));
		if (chn->getUser(usr->getNick()) == NULL)
	 		throw (ErrException(ERR_NOTONCHANNEL(usr->getNick(), usr->getUser(), chn->getName()).c_str()));
		usrVec = chn->getUsers();
		for (int j = 0; j < (int) usrVec.size(); j++)
		{
			tmp = usrVec[j];
			tmp->setWriteBuff(tmp->getWriteBuff() + MSG_PART(usr->getNick(), usr->getUser(), chn->getName(), partMessage));
			srv.setEvent(tmp->getSockFd(), POLLOUT);
		}
		chn->removeUser(usr->getNick());
		if (chn->getUsers().size() == 0)
			srv.removeChannel(chn);
	}
}

void	Commands::serviceCommand(Server &srv, User *usr, std::vector<std::string> params)
{	
	(void) srv;
	if (usr->getReg() % 2 == 0)
		throw (ErrException(ERR_NOTREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if ((usr->getReg() >> 3) % 2 == 1 || (usr->getReg() >> 2) % 2 == 1 || (usr->getReg() >> 1) % 2 == 1)
		throw (ErrException(ERR_ALREADYREGISTERED(usr->getNick(), usr->getUser()).c_str()));
	if (!User::checkNick(params[0]))
		throw (ErrException(ERR_ERRONEUSNICKNAME(usr->getNick(), usr->getUser()).c_str()));
	if (params.size() < 6)
		throw (ErrException(ERR_NEEDMOREPARAMS(usr->getNick(), usr->getUser(), SERVICE).c_str()));
	usr->setNick(ft_tolower(params[0]));
	usr->setReg(usr->getReg() | 8);
	usr->setInfoBot(params[5]);
	MY_DEBUG(">> SERVICE command executed" << std::endl << "usr->sfd [" << usr->getSockFd() << "] "
		<< "usr->nick [" << usr->getNick() << "] ")
	usr->setWriteBuff(usr->getWriteBuff() + RPL_WELCOME(usr->getNick(), usr->getUser(), SRV_NAME, usr->getIp()));
	usr->setWriteBuff(usr->getWriteBuff() + RPL_YOURHOST(usr->getNick()));
	MY_DEBUG(">> SERVICE registered correctly from NICK command")
}
