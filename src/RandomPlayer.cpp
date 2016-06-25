#include "RandomPlayer.hpp"
#include "Board.hpp"

#include <random>
#include <chrono>
#include <vector>
#include <stdexcept>


size_t RandomPlayer::makeMove(const Board& b, Board::Player p) {
	using namespace std;
	(void)p;

	auto& moves = b.legalMoves();
	if(moves.empty()) {
		throw invalid_argument("No possible legal moves");
	}

	static thread_local default_random_engine g(chrono::high_resolution_clock::now().time_since_epoch().count());
	uniform_int_distribution<int> d(0, moves.size() - 1);

	return moves[d(g)];
}
