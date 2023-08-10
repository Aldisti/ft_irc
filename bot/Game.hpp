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

class Game
{
	private:
		char		_mat[3][3];
		std::string	_p1;
		std::string	_p2;
		int			_turn;
		bool		_end;

	public:
		Game(std::string p1, std::string p2);
		~Game(void);
		Game(Game const &gm);
		Game &operator=(Game const &gm);

		// getters
		
};
