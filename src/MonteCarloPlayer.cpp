#include "MonteCarloPlayer.hpp"
#include "RandomPlayer.hpp"
#include "Board.hpp"
#include "Game.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

MonteCarloPlayer::MonteCarloPlayer(size_t maxGames) : _maxGames(maxGames) {}

size_t MonteCarloPlayer::makeMove(const Board& b, Board::Player p) {
	if(p == Board::Player::E) {
		throw std::invalid_argument("Passed E as player");
	}

	Board::Player o = p == Board::Player::P1 ? Board::Player::P2 : Board::Player::P1;

	auto& moves = b.legalMoves();
	if(moves.size() == 0) {
		throw std::invalid_argument("No legal moves available");
	}
	std::vector<int> scores(moves.size());

	size_t gamesPerMove = _maxGames / moves.size();
	
	
	#pragma omp parallel for
	for(size_t move = 0; move < moves.size(); move++) {
		int score = 0;
		
		Board moved(b);
		size_t t = moved.put(p, moves[move]);
		if(causedWin(moved, moves[move], t)) {
			scores[move] = gamesPerMove;
			continue;
		} else if(moved.isFull()) {
			continue;
		}

		Game g(moved, std::unique_ptr<Player>(new RandomPlayer()), std::unique_ptr<Player>(new RandomPlayer()));

		for(size_t i = 0; i < gamesPerMove; i++) {
			g.board() = moved;
			g.toMove() = o;
			
			Board::Player winner;
			while((winner = g.step()) == Board::Player::NONE);

			if(winner == p) {
				score++;
			} else if(winner == o) {
				score--;
			}
		}

		scores[move] = score;
	}

	int best = -gamesPerMove - 1;
	size_t bestMove = -1;
	for(size_t i = 0; i < scores.size(); i++) {
		if(scores[i] >= best) {
			best = scores[i];
			bestMove = moves[i];
		}
	}

	return bestMove;
}



/*
static double minimax(Board& b, size_t lvl, Board::Player p, Board::Player current) {
	if(lvl == 0) {
		return heuristic(b, p);
	}
	
	vector<size_t> legalMoves;
	for(size_t i = 0; i < b.width(); i++) {
		if(!b.isColumnFull(i)) {
			legalMoves.push_back(i);
		}
	}

	if(current == p) {
		double best = -1.0 / 0.0; // -inf
		for(size_t move : legalMoves) {
			size_t y = b.put(cur, move);
			if(causedWin(move, y)) {
				best = 1.0 / 0.0;
				b.unput(move);
				break;
			}

			best = std::max(best, minimax(b, lvl-1, p, current == Board::Player::P1 ? P2 : P1));
			b.unput(move);
		}

		return best;
	} else {
		double worst = 1.0 / 0.0; // +inf
		for(size_t move : legalMoves) {
			size_t y = b.put(cur, move);
			if(causedWin(move, y)) {
				worst = -1.0 / 0.0;
				b.unput(move);
				break;
			}

			best = std::min(worst, minimax(b, lvl-1, p, current == Board::Player::P1 ? P2 : P1));
			b.unput(move);
		}

		return worst;
	}

	//inconceivable
	return 0.0;
}
*/

