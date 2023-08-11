/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adi-stef <adi-stef@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 11:24:52 by gpanico           #+#    #+#             */
/*   Updated: 2023/08/11 15:13:43 by adi-stef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

std::map<char, std::string>	Game::values;

Game::Game(std::string p1, std::string p2): _p1(p1), _p2(p2)
{
	this->_turn = 1;
	this->_end = false;
	this->_mat = new char*[3];
	for (int i = 0; i < 3 ; i++)
	{
		this->_mat[i] = new char[3];
		for (int j = 0; j < 3; j++)
		{
			this->_mat[i][j] = EMPTY;
			this->_slots.push_back(i * 10 + j);
		}
	}
	if (Game::values.size() == 0)
	{
		Game::values[EMPTY] = std::string("  ");
		Game::values[CROSS] = std::string("x");
		Game::values[CIRCLE] = std::string("o");
	}
}

Game::Game(Game const &gm)
{
	*this = gm;
}

Game	&Game::operator=(Game const &gm)
{
	if(this == &gm)
		return (*this);
	for (int i = 0; i < 3; i++)
		delete [] this->_mat[i];
	delete [] this->_mat;
	this->_mat = gm.getMat();
	this->_p1 = gm.getP1();
	this->_p2 = gm.getP2();
	this->_turn = gm.getTurn();
	this->_end = gm.getEnd();
	this->_slots = gm.getSlots();
	return (*this);
}

Game::~Game(void)
{
	for (int i = 0; i < 3; i++)
		delete [] this->_mat[i];
	delete [] this->_mat;
}

char	**Game::getMat(void) const
{
	return (this->_mat);
}

std::string	Game::getP1(void) const
{
	return (this->_p1);
}

std::string	Game::getP2(void) const
{
	return (this->_p2);
}

int	Game::getTurn(void) const
{
	return (this->_turn);
}

bool	Game::getEnd(void) const
{
	return (this->_end);
}

std::vector<int> Game::getSlots(void) const
{
	return (this->_slots);
}

void	Game::setP2(std::string p2)
{
	this->_p2 = p2;
}

bool	Game::checkVictory(void) const
{
	if (this->checkRows() || this->checkCols() || this->checkDiags())
		return (true);
	return (false);
}

bool	Game::checkRows(void) const
{
	for (int i = 0; i < 3; i++)
		if (this->_mat[i][0] == this->_mat[i][1]
				&& this->_mat[i][1] == this->_mat[i][2] && this->_mat[i][0] != EMPTY)
			return (true);
	return (false);
}

bool	Game::checkCols(void) const
{
	for (int i = 0; i < 3; i++)
		if (this->_mat[0][i] == this->_mat[1][i]
				&& this->_mat[1][i] == this->_mat[2][i] && this->_mat[0][i] != EMPTY)
			return (true);
	return (false);
}

bool	Game::checkDiags(void) const
{
	if (this->_mat[0][0] == this->_mat[1][1]
			&& this->_mat[1][1] == this->_mat[2][2] && this->_mat[1][1] != EMPTY)
			return (true);
	else if (this->_mat[2][0] == this->_mat[1][1]
			&& this->_mat[1][1] == this->_mat[0][2] && this->_mat[1][1] != EMPTY)
			return (true);
	return (false);
}

void	Game::makeMove(int i, int j)
{
	i--;
	j--;
	if (i < 0 || i > 2 || j < 0 || j > 2)
		throw (ErrException("Invalid move: out of bounds"));
	else if (this->_mat[i][j] != EMPTY)
		throw (ErrException("Invalid move: cell already full"));
	this->_mat[i][j] = this->_turn == 1 ? CROSS : CIRCLE;
	for (int k = 0; k < (int) this->_slots.size(); k++)
		if (this->_slots[k] == (i * 10 + j))
		{
			this->_slots.erase(this->_slots.begin() + k);
			break ;
		}
	if (this->checkVictory() || this->_slots.size() == 0)
		this->_end = true;
	else
		this->_turn = this->_turn == 1 ? 2 : 1;
}

void	Game::botMove(void)
{
	int	botMove;

	std::random_shuffle(this->_slots.begin(), this->_slots.end());
	botMove = this->_slots.back();
	this->_slots.pop_back();
	this->_mat[botMove / 10][botMove % 10] = CIRCLE;
	if (this->checkVictory() || this->_slots.size() == 0)
		this->_end = true;
	else
		this->_turn = this->_turn == 1 ? 2 : 1;
}

bool	Game::isNickTurn(std::string nick) const
{
	if ((this->_turn == 1 && this->_p1 == nick)
			|| (this->_turn == 2 && this->_p2 == nick))
		return (true);
	return (false);
}

std::vector<std::string>	Game::getMatPrint(std::string player) const
{
	std::vector<std::string>	msg;
	std::string					tmp;

	msg.push_back(BMSG_PRIVMSG(player, std::string("     1    2    3 ")));
	for (int i = 0; i < 3; i++)
	{
		if (i > 0)
			msg.push_back(BMSG_PRIVMSG(player, "     ―+―+― "));
		tmp = Utils::Utils::ft_itoa(i + 1) + std::string("   ");
		for (int j = 0; j < 3; j++)
		{
			tmp += Game::values[this->_mat[i][j]];
			if (j == 0)
				tmp += "  I ";
			else if (j == 1)
				tmp += "  I  ";
		}
		msg.push_back(BMSG_PRIVMSG(player, tmp));
	}
	return (msg);
}
