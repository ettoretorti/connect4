#pragma once

#include "Player.hpp"

class HybridPlayer : public Player {
public:
	HybridPlayer(size_t maxGames, size_t minimaxDepth);
	virtual size_t makeMove(const Board& b, Board::Player p);

private:
	size_t _maxGames;
	size_t _mmDepth;
};
