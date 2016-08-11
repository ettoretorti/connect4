#pragma once

#include <memory>
#include <vector>
#include <string>

class Board {
public:
	enum class Player : unsigned char { E = 0, P1 = 1, P2 = 2, NONE = 3 };
	
	Board(size_t width, size_t height);
	Board(const Board& o);
	Board(Board&& o);
	Board& operator=(const Board& o);
	Board& operator=(Board&& o);
	
	Player operator()(size_t x, size_t y) const;

	void reset();
	
	//! Returns the y coordinate of the inserted piece,
	//! height if the column was already full, or E is inserted
	size_t put(Player p, size_t x);
	void unput(size_t x);
	
	Board::Player winner() const;
	bool isColumnFull(size_t x) const;
	bool isFull() const;
	bool isGameOver() const;
	
	const std::vector<size_t>& legalMoves() const;
	std::string toString() const;

	size_t width() const;
	size_t height() const;

private:
	Player& get(size_t x, size_t y);
	
	std::vector<size_t> _lMovs;
	std::unique_ptr<Player[]> _cells;
	std::unique_ptr<Player*[]> _rowPtrs;
	std::unique_ptr<size_t[]> _colHeight;
	size_t _width;
	size_t _height;
};

//! More efficient than calling winner() if the position of
//! the last move is known
bool causedWin(const Board& b, size_t x, size_t y);

