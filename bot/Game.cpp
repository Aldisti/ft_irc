#include "Game.hpp"

Game::Game(std::string p1, std::string p2): _p1(p1), _p2(p2)
{
	this->_turn = 1;
	this->_end = false;
	this->_mat = new char*[3];
	for (int i = 0; i < 3 ; i++)
	{
		this->_mat[i] = new char[3];
		memset((void *) this->_mat[i], EMPTY, 3);
	}
	return ;
}

Game::Game(Game const &gm)
{
	for (int i = 0; i < 3; i++)
		delete [] this->_mat[i];
	delete [] this->_mat;
	*this = gm;
}

Game	&Game::operator=(Game const &gm)
{
	if(this == &gm)
		return (*this);
	this->_mat = gm.getMat();
	this->_p1 = gm.getP1();
	this->_p2 = gm.getP2();
	this->_turn = gm.getTurn();
	this->_end = gm.getEnd();
	return (*this);
}

Game::~Game(void)
{
	return ;
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

bool	Game::checkVictory(void)
{
	if (this->checkRows() || this->checkCols() || this->checkDiags())
		return (true);
	return (false);
}

bool	Game::checkRows(void) const
{
	for (int i = 0; i < 3; i++)
		if (this->_mat[i][0] == this->_mat[i][1]
				&& this->_mat[i][1] == this->_mat[i][2])
			return (true);
	return (false);
}

bool	Game::checkCols(void) const
{
	for (int i = 0; i < 3; i++)
		if (this->_mat[0][i] == this->_mat[1][i]
				&& this->_mat[1][i] == this->_mat[2][i])
			return (true);
	return (false);
}

bool	Game::checkDiags(void) const
{
	if (this->_mat[0][0] == this->_mat[1][1]
			&& this->_mat[1][1] == this->_mat[2][2])
			return (true);
	else if (this->_mat[2][0] == this->_mat[1][1]
			&& this->_mat[1][1] == this->_mat[0][2])
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
	if (this->checkVictory())
		this->_end = true;
	else
		this->_turn = this->_turn == 1 ? 2 : 1;
}
