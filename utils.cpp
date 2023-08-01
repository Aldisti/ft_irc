/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 10:46:40 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/01 11:32:14 by gpanico          ###   ########.fr       */
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
