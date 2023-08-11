/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 14:08:44 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/11 14:48:06 by adi-stef         ###   ########.fr       */
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
		static std::map<char, std::string>	values;
		char								**_mat;
		std::string							_p1;
		std::string							_p2;
		int									_turn;
		bool								_end;
		std::vector<int>					_slots;

		// functions
		bool	checkRows(void) const;
		bool	checkCols(void) const;
		bool	checkDiags(void) const;

	public:
		Game(std::string p1, std::string p2);
		~Game(void);
		Game(Game const &gm);
		Game &operator=(Game const &gm);

		// getters
		char				**getMat(void) const;
		std::string			getP1(void) const;
		std::string			getP2(void) const;
		int					getTurn(void) const;
		bool				getEnd(void) const;
		std::vector<int>	getSlots(void) const;

		// setters
		void	setP2(std::string p2);

		// functions
		bool						checkVictory(void) const;
		void						makeMove(int i, int j);
		void						botMove(void);
		bool						isNickTurn(std::string nick) const;
		std::vector<std::string>	getMatPrint(std::string player) const;
};
