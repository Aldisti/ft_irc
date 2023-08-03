/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 10:46:40 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/02 11:43:16 by gpanico          ###   ########.fr       */
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
