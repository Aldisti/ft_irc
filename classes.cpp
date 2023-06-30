#include "classes.hpp"

bool	User::checkNick(std::string nick)
{
	if (nick.size() < 1 || nick.size() > 9)
		return (false);
	if (LETTERS.find(nick[0]) == NPOS && SPECIAL.find(nick[0]) == NPOS)
		return (false);
	for (int i = 1; i < (int) nick.size(); i++)
		if (LETTERS.find(nick[0]) == NPOS && SPECIAL.find(nick[0]) == NPOS
				&& DIGITS.find(nick[0]) == NPOS || nick[0] != '-')
			return (false);
	return (true);
}

User::User(int sockfd): _sockfd(sockfd), _registered(0)
{
	return ;
}

User::User(User const &usr)
{
	*this = usr;
}

User::~User(void)
{
	close(this->_sockfd);
}
