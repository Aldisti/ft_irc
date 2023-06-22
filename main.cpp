/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:17:48 by adi-stef          #+#    #+#             */
/*   Updated: 2023/06/22 16:25:58 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int main(void)
{
    struct sockaddr_storage	their_addr;
    socklen_t				addr_size;
    struct addrinfo			hints, *res;
    int						sockfd, new_fd, n;
	char					buf[1000];

    // !! don't forget your error checking for these calls !!
    // first, load up address structs with getaddrinfo():

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if (getaddrinfo(NULL, MYPORT, &hints, &res) == -1)
		perror("");

    // make a socket, bind it, and listen on it:
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    listen(sockfd, BACKLOG);

    // now accept an incoming connection:

    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

	while (42)
	{
		n = recv(new_fd, buf, 1000, 0);
		if (n < 0)
			return (15);
		if (!n)
			break ;
		std::cout << "Received: " << buf << std::endl;
	}
	std::cout << "Connection closed" << std::endl;
	close(sockfd);
	close(new_fd);
	return (0);
}
