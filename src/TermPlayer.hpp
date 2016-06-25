#pragma once

#include "Player.hpp"

class TermPlayer : public Player {
	virtual size_t makeMove(const Board& b, Board::Player p);
};
