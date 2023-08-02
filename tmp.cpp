/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 12:17:25 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/02 09:59:35 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

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
		if ((i = line.find(' ', j)) == NPOS)
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

int	main(void)
{
	parse(":fiffo 1 2");
	parse(":fiffo 1 2 3 4 5 :ciao come stai");
	parse(":fiffo 0 1 2 3 4 5 6 7 8 9 1 2 3 4 5 :ciao come stai");
	parse("");
	//     01234567890123456789012345678901234
	return (0);
}
