/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:17:48 by adi-stef          #+#    #+#             */
/*   Updated: 2023/06/26 16:25:11 by adi-stef         ###   ########.fr       */
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
	struct pollfd			poll_s[50];
	unsigned short			poll_n = 0;

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

	poll_s[0].fd = sockfd;
	poll_s[0].events = POLLIN;
	poll_s[0].revents = 0;
	poll_n++;

    // now accept an incoming connection:

    // addr_size = sizeof their_addr;
    // new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

	while (42)
	{
		std::cout << "Polling..." << std::endl;
		switch (poll(poll_s, poll_n, 1000))
		{
			case -1:
				std::cout << "poll error" << std::endl;
				return (15);
				break;
			case 0:
				continue ;
		}
		std::cout << "poll_s[0].revents == " << poll_s[0].revents << std::endl;
		if (poll_s[0].revents == POLLIN)
		{
			addr_size = sizeof their_addr;
			std::cout << "Accepting..." << std::endl;
    		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
			std::cout << "Accept done" << std::endl;
			poll_s[poll_n].fd = new_fd;
			poll_s[poll_n].events = POLLIN | POLLOUT;
			poll_s[poll_n].revents = 0;
			poll_n++;
			// continue ;
		}
		for (int i = 1; i < poll_n; i++)
		{
			std::cout << "fd: " << poll_s[i].fd << " - " << poll_s[i].revents << std::endl;
			if (poll_s[i].revents == POLLOUT)
			{
				std::cout << "Receiving: " << i << std::endl;
				n = recv(new_fd, buf, 1000, MSG_DONTWAIT);
				std::cout << "Received: " << buf << std::endl;
			}
		}
	}
	std::cout << "Connection closed" << std::endl;
	close(sockfd);
	close(new_fd);
	return (0);
}
