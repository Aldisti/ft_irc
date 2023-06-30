/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 12:17:25 by adi-stef          #+#    #+#             */
/*   Updated: 2023/06/30 17:24:21 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	parse(std::string line)
{
	std::vector<std::string>	arr;
	int		i = 0;
	int		j = 0;
	int		end = 0;

	// if (line.back() != 10)
	// 	return ;
	// std::cout << line << std::endl;
	if (line[0] == ':')
	{
		arr.push_back(line.substr(1, line.find(32) - 1));
		i = line.find(32);
		while (line[i] == 32 && i < (int)line.size()) i++;
		j = i++;
	}
	end = (line.find(':', i) != NPOS) ? line.find(':', i) : (int)line.size();
	// std::cout << "end: " << end << std::endl;
	while (i < end)
	{
		i = line.find(32, j);
		if (i == NPOS)
			i = line.size();
		arr.push_back(line.substr(j, i - j));
		while (line[i] == 32 && i < (int)line.size()) i++;
		j = i++;
	}
	// std::cout << "Ciclo finito con: [" << j << "] [" << i << "]" << std::endl;
	if (line[j] == ':')
		arr.push_back(line.substr(i, line.size()));
	for (int i = 0; i < (int)arr.size(); i++)
		std::cout << "|" << arr[i] << "| ";
	std::cout << std::endl;
}

int	main(void)
{
	parse(":fiffo ciao     come  stai :a b c");
	//     01234567890123456789012345678901234
	return (0);
}
