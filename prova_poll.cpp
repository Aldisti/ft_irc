#include <iostream>
#include <string>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/types.h>

int	main(void)
{
	struct addrinfo		hints;
	struct addrinfo		*res;
	struct sockaddr		their_addr;
	socklen_t		sin_addr = sizeof(their_addr);
	struct pollfd		pollfds[50];
	char			buff[1001];
	int			buff_size = 1000;
	int			npollfds = 1;
	int			err;
	int			sfd, nsfd;
	int			rs;
	int			r;

	memset((void *) &hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = AI_PASSIVE;
	err = getaddrinfo(NULL, "8000", &hints, &res);
	if (err)
	{
		std::cerr << "getaddrinfo() failed" << std::endl;
		return (err);
	}
	sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sfd == -1)
	{
		std::cerr << "socket() failed" << std::endl;
		return (1);
	}
	if (bind(sfd, res->ai_addr, res->ai_addrlen))
	{
		std::cerr << "bind() failed" << std::endl;
		return (1);
	}
	freeaddrinfo(res);
	if (listen(sfd, 50))
	{
		std::cerr << "listen() failed" << std::endl;
		close(sfd);
		return (1);
	}
	memset((void *) pollfds, 0, sizeof(pollfds));
	pollfds[0].fd = sfd;
	pollfds[0].events = POLLIN;
	for (int k = 0; k < 100; k++)
	{
		std::cout << "polling..." << std::endl;
		rs = poll(pollfds, npollfds, 1000);
		if (rs < 0)
		{
			std::cerr << "poll() failed" << std::endl;
			close(sfd);
			return (1);
		}
		else if (rs == 0)
			continue ;
		for (int i = 0; i < npollfds && rs > 0; i++)
		{
			if (pollfds[i].revents != POLLIN || pollfds[i].fd == -1)
				continue ;
			std::cout << "POLLIN" << std::endl;
			if (pollfds[i].fd == sfd)
			{
				pollfds[npollfds].fd = accept(sfd, &their_addr, &sin_addr);
				if (pollfds[npollfds].fd == -1)
				{
					std::cerr << "accept() failed" << std::endl;
					pollfds[npollfds].fd = 0;
					continue ;
				}
				pollfds[npollfds].events = POLLIN;
				npollfds++;
			}
			else
			{
				memset((void *) buff, 0, buff_size + 1);
				r = recv(pollfds[i].fd, buff, buff_size, MSG_DONTWAIT);
				if (r < 0)
				{
					std::cerr << "recv() failed" << std::endl;
					close(pollfds[i].fd);
					pollfds[i].fd = -1;
					continue ;
				}
				else if (r == 0)
				{
					std::cerr << "connection closed" << std::endl;
					close(pollfds[i].fd);
					pollfds[i].fd = -1;
					continue ;
				}
				if (npollfds > 2)
				{
					if (i + 1 != npollfds)
						send(pollfds[i + 1].fd, buff, buff_size, MSG_DONTWAIT);
					else
						send(pollfds[i - 1].fd, buff, buff_size, MSG_DONTWAIT);
				}
				else
					send(pollfds[i].fd, buff, buff_size, MSG_DONTWAIT);
			}
		}
	}
	for (int i = 0; i < npollfds; i++)
		if (pollfds[i].fd != -1)
			close(pollfds[i].fd);
	return (0);
}
