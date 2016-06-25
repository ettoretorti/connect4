#pragma once

#include "Player.hpp"
#include <vector>

class RandomPlayer : public Player {
public:
	virtual size_t makeMove(const Board& b, Board::Player p);
};
