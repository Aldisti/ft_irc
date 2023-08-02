/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 10:46:40 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/02 10:17:00 by gpanico          ###   ########.fr       */
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
		i = str.find(del, j);
		if (i == (int) NPOS)
		{
			splitted.push_back(str.substr(j));
			break ;
		}
		splitted.push_back(str.substr(j, i - j));
		j = i + 2;
	}
	return (splitted);
}

std::vector<std::string>	ft_parse(std::string line)
{
	std::vector<std::string>	arr;
	int		i = 0;
	int		j = 0;
	int		end = 0;

	if (line[0] == ':')
	{
		i = line.find(' ');
		j++;
		arr.push_back(line.substr(j, i - j));
		j = ++i;
	}
	end = (line.find(':', i) != NPOS) ? line.find(':', i) : (int)line.size();
	while (i < end)
	{
		if ((i = line.find(' ', j)) == (int) NPOS)
			i = line.size();
		arr.push_back(line.substr(j, i - j));
		j = ++i;
	}
	if (j < (int)line.size() && line[j] == ':')
		arr.push_back(line.substr(i));

	if (arr.size() == 0 || arr.size() > 17)
		throw std::exception();
	else if (arr.size() > 16 && (end == (int)line.size() || line[0] != ':'))
		throw std::exception(); // throw exception
	else if (arr.size() > 15 && (end == (int)line.size() && line[0] != ':'))
		throw std::exception(); // throw exception
	for (int i = 0; i < (int)arr.size(); i++) {
		if (!arr[i].size())
			throw std::exception(); // throw exception
		if ((arr[i].find(' ') != NPOS && arr[i] != arr.back()) || (arr[i].find(' ') != NPOS && arr[i] == arr.back() && end == (int)line.size()))
			throw std::exception(); // throw exception
	}
	return (arr);
}
