#include "HybridPlayer.hpp"
#include "RandomPlayer.hpp"
#include "Board.hpp"
#include "Game.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

HybridPlayer::HybridPlayer(size_t maxGames, size_t minimaxDepth)
	: _maxGames(maxGames), _mmDepth(minimaxDepth)
{}

static float minimax(Board& b, size_t lvl, float alpha, float beta, Board::Player p, Board::Player current);

size_t HybridPlayer::makeMove(const Board& b, Board::Player p) {
	if(p == Board::Player::E) {
		throw std::invalid_argument("Passed E as player");
	}

	Board::Player o = p == Board::Player::P1 ? Board::Player::P2 : Board::Player::P1;

	auto& moves = b.legalMoves();
	if(moves.size() == 0) {
		throw std::invalid_argument("No legal moves available");
	}
	std::vector<float> scores(moves.size());

	size_t gamesPerMove = _maxGames / moves.size();
	
	#pragma omp parallel for
	for(size_t move = 0; move < moves.size(); move++) {
		float score = 0;

		Board moved(b);
		size_t t = moved.put(p, moves[move]);
		if(causedWin(moved, moves[move], t)) {
			scores[move] = gamesPerMove;
			continue;
		} else if(moved.isFull()) {
			continue;
		}

		float mmScore = minimax(moved, _mmDepth, -1.0/0.0, 1.0/0.0, p, o);
		if(mmScore == 1.0 / 0.0) {
			scores[move] = mmScore;
			continue;
		} else if(mmScore != 0) {
			scores[move] = mmScore;
			continue;
		}
		
		Game g(moved, std::unique_ptr<Player>(new RandomPlayer()), std::unique_ptr<Player>(new RandomPlayer()));

		for(size_t i = 0; i < gamesPerMove; i++) {
			g.board() = moved;
			g.toMove() = o;
			
			while(!g.step());
			
			auto winner = g.board().winner();

			if(winner == p) {
				score++;
			} else if(winner == o) {
				score--;
			}
		}

		scores[move] = score;
	}

	float best = -1.0 / 0.0;
	size_t bestMove = -1;
	for(size_t i = 0; i < scores.size(); i++) {
		if(scores[i] >= best) {
			best = scores[i];
			bestMove = moves[i];
		}
	}

	return bestMove;
}

static float minimax(Board& b, size_t lvl, float alpha, float beta, Board::Player p, Board::Player current) {
	if(lvl == 0) {
		return 0;
	}
	
	std::vector<size_t> legalMoves = b.legalMoves();

	if(current == p) {
		float best = -1.0 / 0.0; // -inf
		for(size_t move : legalMoves) {
			size_t y = b.put(current, move);
			if(causedWin(b, move, y)) {
				best = 1.0 / 0.0;
				b.unput(move);
				break;
			}

			best = std::max(best, minimax(b, lvl-1, alpha, beta, p, current == Board::Player::P1 ? Board::Player::P2 : Board::Player::P1));
			alpha = std::max(alpha, best);
			b.unput(move);

			if(beta <= alpha) break;
		}

		return best;
	} else {
		float worst = 1.0 / 0.0; // +inf
		for(size_t move : legalMoves) {
			size_t y = b.put(current, move);
			if(causedWin(b, move, y)) {
				worst = -1.0 / 0.0;
				b.unput(move);
				break;
			}

			worst = std::min(worst, minimax(b, lvl-1, alpha, beta, p, current == Board::Player::P1 ? Board::Player::P2 : Board::Player::P1));
			beta = std::min(beta, worst);
			b.unput(move);

			if(beta <= alpha) break;
		}

		return worst;
	}
}

