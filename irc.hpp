/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:21:38 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/08 11:21:25 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cctype>
#include <csignal>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <map>
#include <ctime>

#define DEBUG
#define DEBUG_B 1
#define MY_DEBUG(string) if(DEBUG_B) \
	std::cout << string << std::endl;
#define MYPORT "8001"  // the port users will be connecting to
#define IP std::string("10.12.3.5")
#define SRV_NAME std::string("hcierVI")
#define OPER_PASSWORD std::string("admin")

// utils
#define BUFFSIZE 1024
#define BACKLOG 50     // how many pending connections queue will hold
#ifdef DEBUG
# define TIMEOUT 1000
#else
# define TIMEOUT 2147483647
#endif
#define NPOS std::string::npos
#define PING_TIMEOUT 15 * 1000

// mode
#define WALLOP 1
#define OPERATOR 2
#define F_AWAY 4
#define VALID_MODES std::string("woOa")

// commands
#define CAP std::string("CAP")
#define PASS std::string("PASS")
#define NICK std::string("NICK")
#define USER std::string("USER")
#define PING std::string("PING")
#define PONG std::string("PONG")
#define QUIT std::string("QUIT")
#define OPER std::string("OPER")
#define MODE std::string("MODE")
#define PRIVMSG std::string("PRIVMSG")
#define WALLOPS std::string("WALLOPS")
#define SQUIT std::string("SQUIT")
#define KILL std::string("KILL")
#define AWAY std::string("AWAY")
#define JOIN std::string("JOIN")

// messages
#define MSG_CAP std::string("CAP * LS\r\n")
#define MSG_PONG(ip) std::string("PONG :" + ip + "\r\n")
#define MSG_PING std::string("PING :" + SRV_NAME + "\r\n")
#define MSG_ERROR(message) std::string(":" + SRV_NAME + " ERROR :" + message + "\r\n")
#define MSG_KILL(nick, message) std::string(":" + nick + " KILL :" + message + DEL)
#define MSG_AWAY std::string("away")
#define MSG_JOIN(channel) std::string(DOMAIN + " JOIN :" + channel + DEL)

// irc format
#define CHANNEL std::string("#&+!")
#define LETTERS std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
#define DIGITS std::string("0123456789")
#define HEXDIGITS std::string("0123456789ABCDEF")
#define SPECIAL std::string("[\\]^_`{|}")
#define DEL	std::string("\r\n")
#define MAX_BUFF 512

#define PREFIX(nick, user) std::string(":" + nick + "!" + user + "@" + SRV_NAME)
#define DOMAIN std::string(":" + SRV_NAME)

// replies/errors
#define RPL_WELCOME(nick, user, server) std::string(PREFIX(nick, user) + " 001 " \
		 + server + " " + nick + "!" + user + "@" + SRV_NAME + "\r\n")
#define RPL_UMODEIS(nick, user, modes) std::string(PREFIX(nick, user) + " 221 :" + modes + DEL)
#define RPL_AWAY(nick, user, awayNick, msg) std::string(PREFIX(nick, user) + " 301 " + awayNick + " :" + msg + DEL)
#define RPL_UNAWAY(nick, user) std::string(PREFIX(nick, user) + " 305 " + "You are no longer marked as being away" + DEL)
#define RPL_NOWAWAY(nick, user) std::string(PREFIX(nick, user) + " 306 " + "You have been marked as being away" + DEL)
#define RPL_NOTOPIC(nick, user, channel) std::string(PREFIX(nick, user) + " 331 " + channel + " :No topic is set" + DEL)
#define RPL_NAMREPLY(nick, user, channel, names) std::string(PREFIX(nick, user) + " 353 " + channel + " :" + names + DEL)
#define RPL_ENDOFNAMES(nick, user, channel) std::string(PREFIX(nick, user) + "366 " + channel + " :End of NAMES list" + DEL)
#define RPL_YOUREOPER(nick, user) std::string(PREFIX(nick, user) + " 381 :You are now an IRC operator" + DEL)
#define ERR_NOSUCHNICK(nick, user, name) std::string(PREFIX(nick, user) + " 401 " + name + " :no such nick" + DEL)
#define ERR_NOSUCHSERVER(nick, user, server) std::string(PREFIX(nick, user) + " 402 " + server + " :no such server\r\n")
#define ERR_NOSUCHCHANNEL(nick, user, channel) std::string(PREFIX(nick, user) + " 403 " + channel + " :No such channel" + DEL)
#define ERR_TOOMANYCHANNELS(nick, user, channel) std::string(PREFIX(nick, user) + "405 " + channel + " :You have joined too many channels" + DEL)
#define ERR_TOOMANYTARGETS(nick, user, target, code, msg) std::string(PREFIX(nick, user) + "407 " + target + " :" + code + " recipients. " + msg + DEL)
#define ERR_NORECIPIENT(nick, user, cmd) std::string(PREFIX(nick, user) + " 411 :No recipient given (" + cmd + ")\r\n")
#define ERR_NOTEXTTOSEND(nick, user) std::string(PREFIX(nick, user) + "412 :No text to send" + DEL)
#define ERR_BADMASK(nick, user, mask) std::string(PREFIX(nick, user) + "415 " + mask + " :Bad Server/host mask" + DEL)
#define ERR_UNKNOWNCOMMAND(nick, user, cmd) std::string(PREFIX(nick, user) + " 421 " + cmd + " :Unknown command\r\n")
#define ERR_NONICKNAMEGIVEN(nick, user) std::string(PREFIX(nick, user) + " 431 :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(nick, user) std::string(PREFIX(nick, user) + " 432 " + nick + " :Erroneous nickname\r\n")
#define ERR_NICKNAMEINUSE(nick, user) std::string(PREFIX(nick, user) + " 433 " + nick + " :Nickname is already in use\r\n")
#define ERR_NOTREGISTERED(nick, user) std::string(PREFIX(nick, user) + " 451 :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(nick, user, cmd) std::string(PREFIX(nick, user) + " 461 " + cmd + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTERED(nick, user) std::string(PREFIX(nick, user) + " 462 :Unauthorized command (already registered)\r\n")
#define ERR_PASSWDMISMATCH(nick, user) std::string(PREFIX(nick, user) + " 464 :Password incorrect" + DEL)
#define ERR_BADCHANMASK(nick, user, channel) std::string(PREFIX(nick, user) + " 476 " + channel + " :Bad Channel Mask" + DEL)
#define ERR_NOPRIVILEGES(nick, user) std::string(PREFIX(nick, user) + " 481 :Permission Denied- You're not an IRC operator" + DEL)
#define ERR_UMODEUNKNOWNFLAG(nick, user) std::string(PREFIX(nick, user) + " 501 :Unknown MODE flag" + DEL)
#define ERR_USERSDONTMATCH(nick, user) std::string(PREFIX(nick, user) + " 502 :Cannot change mode for other users" + DEL)
#define ERR_BADSYNTAX(nick, user) std::string(PREFIX(nick, user) + " 542 :bad syntax\r\n")


#include "utils.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"
