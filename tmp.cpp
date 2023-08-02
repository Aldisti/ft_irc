/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 12:17:25 by adi-stef          #+#    #+#             */
/*   Updated: 2023/08/02 11:40:13 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

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

int	main(void)
{
	try { ft_parse(":fiffo 1 2"); }
	catch (std::exception e) { std::cout << "caught" << std::endl; }
	try { ft_parse(":fiffo 1 2 3 4 5 :ciao come stai"); }
	catch (std::exception e) { std::cout << "caught" << std::endl; }
	try { ft_parse(":fiffo cmd 0 1 2 3 4 5 6 7 8 9 1 2 3 4 5 :ciao come stai"); }
	catch (std::exception e) { std::cout << "caught" << std::endl; }
	try { ft_parse(""); }
	catch (std::exception e) { std::cout << "caught" << std::endl; }
 ft_parse("    :fiffo              1                 2    :   ciao come     stai m     ");
	//     01234567890123456789012345678901234
	return (0);
}
