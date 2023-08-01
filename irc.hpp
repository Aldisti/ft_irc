/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:21:38 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/01 11:28:35 by gpanico          ###   ########.fr       */
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
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <map>

#define MYPORT "8001"  // the port users will be connecting to

// utils
#define BUFFSIZE 8
#define BACKLOG 50     // how many pending connections queue will hold
#define NPOS std::string::npos

// commands
#define PASS std::string("PASS")
#define CAP std::string("CAP")

// messages
#define CAPMSG std::string("CAP LS * :")

// irc format
#define LETTERS std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
#define DIGITS std::string("0123456789")
#define HEXDIGITS std::string("0123456789ABCDEF")
#define SPECIAL std::string("[\]^_`{|}")
#define DEL	std::string("\r\n")
#define MAX_BUFF 512
