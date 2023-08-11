/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 10:46:40 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/11 11:27:51 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "utils.hpp"

std::vector<std::string>	ft_split(std::string str, std::string del)
{
	std::vector<std::string>	splitted;
	int							i = 0;
	int							j = 0;

	while (i < (int) str.size())
	{
		while (str.substr(j, del.size()) == del)
			j += del.size();
		i = str.find(del, j);
		if (i == (int) NPOS)
		{
			if (str.substr(j).size())
				splitted.push_back(str.substr(j));
			break ;
		}
		splitted.push_back(str.substr(j, i - j));
		j = i + del.size();
	}
	return (splitted);
}

std::vector<std::string>	ft_parse(std::string line)
{
	std::vector<std::string>	arr;
	std::string					msg = "";
	int							i = 0;

	if (!line.size())
		throw std::exception();
	while (line[i] == ' ')
		i++;
	if ((i = line.find(':', i + 1)) != (int)NPOS)
	{
		msg = line.substr(i + 1);
		line.resize(i);
	}
	arr = ft_split(line, " ");

	if (arr[0][0] == ':')
		arr[0] = arr[0].substr(1);
	if (msg != "")
		arr.push_back(msg);

	if (arr.size() == 0 || arr.size() > 17)
		throw std::exception();
	if (arr.size() > 16 && line[0] != ':')
		throw std::exception();
	return (arr);
}

int	ft_convertToMode(char c)
{
	switch (c)
	{
		case 'w':
			return (WALLOP);
			break ;
		case 'o':
			return (OPERATOR);
			break ;
		case 'O':
			return (OPERATOR);
			break ;
		case 'a':
			return (F_AWAY);
			break ;
		default:
			return (0);
	}
}

std::string	*ft_strdup(const char *str)
{
	std::string	*copy;

	copy = new std::string(str);
	return (copy);
}

std::string	ft_toupper(std::string str)
{
	for (int i = 0; i < (int) str.size(); i++)
		str[i] = std::toupper(str[i]);
	return (str);
}

std::string	ft_tolower(std::string str)
{
	for (int i = 0; i < (int) str.size(); i++)
	{
		if (UPPER_CHARS.find(str[i]) != NPOS)
			str[i] = LOWER_CHARS[UPPER_CHARS.find(str[i])];
		else
			str[i] = std::tolower(str[i]);
	}
	return (str);
}

long	ft_gettime(void)
{
	time_t	now;

	std::time(&now);
	return (((long)((int) now)));
}

bool	ft_isdigit(std::string str)
{
	for (int i = 0; i < (int) str.length(); i++)
		if (!std::isdigit(str[i]))
			return (false);
	return (true);
}

