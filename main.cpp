/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:17:48 by adi-stef          #+#    #+#             */
/*   Updated: 2023/06/22 16:20:57 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int main(void)
{
    struct sockaddr_storage	their_addr;
    socklen_t				addr_size;
    struct addrinfo			hints, *res;
    int						sockfd, new_fd, n;
	std::string					buf;
	std::string					msg("01234567890123456789");

    // !! don't forget your error checking for these calls !!
    // first, load up address structs with getaddrinfo():

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if (getaddrinfo("10.12.3.3", MYPORT, &hints, &res) == -1)
		perror("");

    // make a socket, bind it, and listen on it:
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    connect(sockfd, res->ai_addr, res->ai_addrlen);
    n = recv(sockfd, (void *) msg.c_str(), 20, 0);
    std::cout << msg << std::endl;


    // ready to communicate on socket descriptor new_fd!

	while (42)
	{
		std::getline(std::cin, buf);
		n = send(sockfd, buf.c_str(), buf.size(), 0);
		if (n < 0)
			return (15);
		if (!n)
			break ;
		std::cout << "Sent: " << n << std::endl;
	}
	std::cout << "Connection closed" << std::endl;
	close(sockfd);
	close(new_fd);
	return (0);
}
