/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:21:38 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/02 14:40:10 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cctype>
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

#define MYPORT "8001"  // the port users will be connecting to

// utils
#define BUFFSIZE 100
#define BACKLOG 50     // how many pending connections queue will hold
#define TIMEOUT 1000
#define NPOS std::string::npos

// commands
#define CAP std::string("CAP")
#define PASS std::string("PASS")
#define NICK std::string("NICK")
#define USER std::string("USER")
#define PING std::string("PING")
#define PONG std::string("PONG")

// messages
#define CAPMSG std::string("CAP * LS")

// irc format
#define LETTERS std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
#define DIGITS std::string("0123456789")
#define HEXDIGITS std::string("0123456789ABCDEF")
#define SPECIAL std::string("[\\]^_`{|}")
#define DEL	std::string("\r\n")
#define MAX_BUFF 512

// replies/errors
#define RPL_WELCOME(nick, user) std::string(":" + nick + "!" + user + "@localhost 001 Welcome =) " + nick + "!" + user + "@localhost")
#define ERR_NOSUCHNICK(nick, user) std::string(":" + nick + "!" + user + "@localhost 401 " + nick + " :no such nick")
#define ERR_UNKNOWNCOMMAND(nick, user, cmd) std::string(":" + nick + "!" + user + "@localhost 421 " + cmd + " :Unknown command")
#define ERR_NONICKNAMEGIVEN(nick, user) std::string(":" + nick + "!" + user + "@localhost 431 :No nickname given")
#define ERR_ERRONEUSNICKNAME(nick, user) std::string(":" + nick + "!" + user + "@localhost 432 " + nick + " :Erroneous nickname")
#define ERR_NICKNAMEINUSE(nick, user) std::string(":" + nick + "!" + user + "@localhost 433 " + nick + " :Nickname is already in use")
#define ERR_NOTREGISTERED(nick, user) std::string(":" + nick + "!" + user + "@localhost 451 :You have not registered")
#define ERR_NEEDMOREPARAMS(nick, user, cmd) std::string(":" + nick + "!" + user + "@localhost 461 " + cmd + " :Not enough parameters")
#define ERR_ALREADYREGISTERED(nick, user) std::string(":" + nick + "!" + user + "@localhost 462 :Unauthorized command (already registered)")
#define ERR_BADSYNTAX(nick, user) std::string(":" + nick + "!" + user + "@localhost 542 :bad syntax")
#define ERR_INCORRECTPASS(nick, user) std::string(":" + nick + "!" + user + "@localhost 543 :incorrect password")
