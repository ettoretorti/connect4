#pragma once

#include "Player.hpp"

class MonteCarloPlayer : public Player {
public:
	MonteCarloPlayer(size_t maxGames);
	virtual size_t makeMove(const Board& b, Board::Player p);

private:
	size_t _maxGames;
};
