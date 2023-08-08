/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 09:37:05 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/07 16:40:34 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc.hpp"

std::vector<std::string>	ft_split(std::string str, std::string del);
std::vector<std::string>	ft_parse(std::string line);
int							ft_convertToMode(char c);
std::string					*ft_strdup(const char *c);
std::string					ft_toupper(std::string str);
long						ft_gettime(void);
