#pragma once

#include "Board.hpp"
#include "Player.hpp"

#include <memory>
#include <vector>

class Game {
public:
	Game(Board&& b, std::unique_ptr<Player> p1, std::unique_ptr<Player> p2);
	Game(const Board& b, std::unique_ptr<Player> p1, std::unique_ptr<Player> p2);

	Board& board();
	const Board& board() const;
	
	const Board::Player& toMove() const;
	Board::Player& toMove();

	bool step();

private:
	Board _board;
	std::unique_ptr<Player> _p1;
	std::unique_ptr<Player> _p2;
	Board::Player _toMove;
};
