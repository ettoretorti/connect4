#include "Game.hpp"

#include <stdexcept>
#include <cassert>

Game::Game(Board&& b, std::unique_ptr<Player> p1, std::unique_ptr<Player> p2)
	: _board(b), _p1(std::move(p1)), _p2(std::move(p2)), _toMove(Board::Player::P1)
{}

Game::Game(const Board& b, std::unique_ptr<Player> p1, std::unique_ptr<Player> p2)
	: _board(b), _p1(std::move(p1)), _p2(std::move(p2)), _toMove(Board::Player::P1)
{}

Board& Game::board() {
	return _board;
}

const Board& Game::board() const {
	return _board;
}

Board::Player& Game::toMove() {
	return _toMove;
}

const Board::Player& Game::toMove() const {
	return _toMove;
}

bool Game::step() {
	assert(!_board.isGameOver());

	size_t move = _toMove == Board::Player::P2 ? _p2->makeMove(_board, _toMove) :
	                                             _p1->makeMove(_board, _toMove);

	if(move >= _board.width() || _board.isColumnFull(move)) {
		throw std::domain_error("Player returned invalid move.");
	}

	size_t y = _board.put(_toMove, move);

	_toMove = _toMove == Board::Player::P2 ? Board::Player::P1 :
	                                         Board::Player::P2;
	
	
	return _board.isFull() || causedWin(_board, move, y);
}
