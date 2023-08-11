/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 14:08:44 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/10 14:46:13 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../server/irc.hpp"
#define EMPTY ' '
#define CROSS 'x'
#define CIRCLE 'o'


class Game
{
	private:
		//variables
		char		**_mat;
		std::string	_p1;
		std::string	_p2;
		int			_turn;
		bool		_end;

		// functions
		bool	checkRows(void) const;
		bool	checkCols(void) const;
		bool	checkDiags(void) const;
		bool	checkVictory(void);

	public:
		Game(std::string p1, std::string p2);
		~Game(void);
		Game(Game const &gm);
		Game &operator=(Game const &gm);

		// getters
		char		**getMat(void) const;
		std::string	getP1(void) const;
		std::string	getP2(void) const;
		int			getTurn(void) const;
		bool		getEnd(void) const;

		// setters
		
		// functions
		void	makeMove(int i, int j);
};
