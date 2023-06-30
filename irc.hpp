/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:21:38 by adi-stef          #+#    #+#             */
/*   Updated: 2023/06/30 14:20:31 by gpanico          ###   ########.fr       */
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
#define BACKLOG 10     // how many pending connections queue will hold

// utils
#define BUFFSIZE 512
#define BACKLOG 50

// commands
#define PASS std::string("PASS")
#define CAP std::string("CAP")

// messages
#define CAPMSG std::string("CAP LS * :")
