#pragma once

#include "Board.hpp"

class Player {
public:
	virtual size_t makeMove(const Board& b, Board::Player p) = 0;
};
