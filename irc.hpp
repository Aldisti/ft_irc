/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:21:38 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/02 15:57:30 by gpanico          ###   ########.fr       */
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
#define IP std::string("10.12.3.3")
#define SRV_NAME std::string("ircsrv")

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
#define MSG_CAP std::string("CAP * LS\r\n")
#define MSG_PONG(ip) std::string("PONG :" + ip + "\r\n")

// irc format
#define LETTERS std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
#define DIGITS std::string("0123456789")
#define HEXDIGITS std::string("0123456789ABCDEF")
#define SPECIAL std::string("[\\]^_`{|}")
#define DEL	std::string("\r\n")
#define MAX_BUFF 512

// replies/errors
#define RPL_WELCOME(nick, user, server) std::string(":" + nick + "!" + user + "@localhost 001 " + server + " " + nick + "!" + user + "@localhost\r\n")
#define ERR_NOSUCHNICK(nick, user) std::string(":" + nick + "!" + user + "@localhost 401 " + nick + " :no such nick\r\n")
#define ERR_NOSUCHSERVER(nick, user, server) std::string(":" + nick + "!" + user + "@localhost 402 " + server + " :no such server\r\n")
#define ERR_UNKNOWNCOMMAND(nick, user, cmd) std::string(":" + nick + "!" + user + "@localhost 421 " + cmd + " :Unknown command\r\n")
#define ERR_NONICKNAMEGIVEN(nick, user) std::string(":" + nick + "!" + user + "@localhost 431 :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(nick, user) std::string(":" + nick + "!" + user + "@localhost 432 " + nick + " :Erroneous nickname\r\n")
#define ERR_NICKNAMEINUSE(nick, user) std::string(":" + nick + "!" + user + "@localhost 433 " + nick + " :Nickname is already in use\r\n")
#define ERR_NOTREGISTERED(nick, user) std::string(":" + nick + "!" + user + "@localhost 451 :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(nick, user, cmd) std::string(":" + nick + "!" + user + "@localhost 461 " + cmd + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTERED(nick, user) std::string(":" + nick + "!" + user + "@localhost 462 :Unauthorized command (already registered)\r\n")
#define ERR_BADSYNTAX(nick, user) std::string(":" + nick + "!" + user + "@localhost 542 :bad syntax\r\n")
#define ERR_INCORRECTPASS(nick, user) std::string(":" + nick + "!" + user + "@localhost 543 :incorrect password\r\n")
