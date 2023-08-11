/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 09:37:05 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/11 11:28:29 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc.hpp"

std::vector<std::string>	ft_split(std::string str, std::string del);
std::vector<std::string>	ft_parse(std::string line);
int							ft_convertToMode(char c);
std::string					*ft_strdup(const char *c);
std::string					ft_toupper(std::string str);
std::string					ft_tolower(std::string str);
long						ft_gettime(void);
bool						ft_isdigit(std::string str);

template <typename T>
std::string	ft_itoa(T num)
{
	std::ostringstream ss;
	ss << num;
	return (ss.str());
}
