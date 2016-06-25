#include "Board.hpp"

#include <cstring>
#include <cassert>
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <sstream>

Board::Board(size_t width, size_t height)
	: _lMovs(),
          _cells(std::unique_ptr<Player[]>(new Player[width * height]())),
	  _rowPtrs(std::unique_ptr<Player*[]>(new Player*[width])),
	  _colHeight(std::unique_ptr<size_t[]>(new size_t[width]())),
	  _width(width), _height(height)
{
	assert(height != 0 && ((width * height) / height) == width);

	for(size_t row = 0; row < height; row++) {
		_rowPtrs[row] = &_cells[row * width];
	}

	for(size_t i = 0; i < width; i++) {
		_lMovs.push_back(i);
	}
}

Board::Board(const Board& o)
	: _lMovs(o._lMovs),
          _cells(std::unique_ptr<Player[]>(new Player[o._width * o._height])),
	  _rowPtrs(std::unique_ptr<Player*[]>(new Player*[o._width])),
	  _colHeight(std::unique_ptr<size_t[]>(new size_t[o._width])),
	  _width(o._width), _height(o._height)
{
	for(size_t i = 0; i < _width * _height; i++) {
		_cells[i] = o._cells[i];
	}

	for(size_t i = 0; i < _height; i++) {
		_rowPtrs[i] = &_cells[i * _width];
	}
	
	for(size_t i = 0; i < _width; i++) {
		_colHeight[i] = o._colHeight[i];
	}
}

Board::Board(Board&& o)
	: _lMovs(std::move(o._lMovs)), _cells(std::move(o._cells)), _rowPtrs(std::move(o._rowPtrs)),
          _colHeight(std::move(o._colHeight)), _width(o._width), _height(o._height)
{
	o._width = 0;
	o._height = 0;
}

Board& Board::operator=(const Board& o) {
	if(!(_width <= o._width && _height <= o._height)) {
		_cells.reset(new Player[o._width * o._height]);
		_rowPtrs.reset(new Player*[o._height]);
		_colHeight.reset(new size_t[o._width]);
	}

	_width = o._width;
	_height = o._height;
	_lMovs = o._lMovs;

	for(size_t i = 0; i < o._width * o._height; i++) {
		_cells[i] = o._cells[i];
	}
	
	for(size_t i = 0; i < o._height; i++) {
		_rowPtrs[i] = &_cells[i * _width];
	}

	for(size_t i = 0; i < o._width; i++) {
		_colHeight[i] = o._colHeight[i];
	}

	return *this;
}

Board& Board::operator=(Board&& o) {
	_cells = std::move(o._cells);
	_colHeight = std::move(o._colHeight);
	_lMovs = std::move(o._lMovs);

	_width = o._width;
	_height = o._height;

	o._width  = 0;
	o._height = 0;

	return *this;
}

size_t Board::width() const {
	return _width;
}

size_t Board::height() const {
	return _height;
}

Board::Player& Board::get(size_t x, size_t y) {
	assert(x < _width);
	assert(y < _height);

	return _rowPtrs[y][x];
}

Board::Player Board::operator()(size_t x, size_t y) const {
	assert(x < _width);
	assert(y < _height);

	return _rowPtrs[y][x];
}

void Board::reset() {
	size_t total = _width * _height;
	for(size_t i = 0; i < total; i++) {
		_cells[i] = Player::E;
	}
}

size_t Board::put(Board::Player p, size_t x) {
	assert(x < _width);
	
	if(p == Player::E) {
		return _height;
	}

	if(_colHeight[x] < _height) {
		get(x, _colHeight[x]++) = p;
		if(_colHeight[x] >= _height) {
			for(auto it = _lMovs.begin(); it != _lMovs.end(); ++it) {
				if(*it == x) {
					_lMovs.erase(it);
					break;
				}
			}
		}

		return _colHeight[x] - 1;
	}
	
	return _height;
}

void Board::unput(size_t x) {
	assert(_colHeight[x] != 0);
	
	if(_colHeight[x] >= _height) {
		_lMovs.push_back(x);
	}
	get(x, --_colHeight[x]) = Player::E;
}

bool Board::isColumnFull(size_t x) const {
	assert(x < _width);
	return _colHeight[x] >= _height;
}

bool Board::isFull() const {
	for(size_t i = 0; i < _width; i++) {
		if(_colHeight[i] < _height) {
			return false;
		}
	}

	return true;
}

static inline bool checkHorizontal(const Board& b, size_t x, size_t y) {
	auto one   = b(x-0, y);
	auto two   = b(x-1, y);
	auto three = b(x-2, y);
	auto four  = b(x-3, y);

	return (one == two) & (two == three) & (three == four);
}

static inline bool checkVertical(const Board& b, size_t x, size_t y) {
	auto one   = b(x, y-0);
	auto two   = b(x, y-1);
	auto three = b(x, y-2);
	auto four  = b(x, y-3);

	return (one == two) & (two == three) & (three == four);
}

static inline bool checkUDiag(const Board& b, size_t x, size_t y) {
	auto one   = b(x-0, y-0);
	auto two   = b(x-1, y-1);
	auto three = b(x-2, y-2);
	auto four  = b(x-3, y-3);

	return (one == two) & (two == three) & (three == four);
}

static inline bool checkLDiag(const Board& b, size_t x, size_t y) {
	auto one   = b(x-0, y+0);
	auto two   = b(x-1, y+1);
	auto three = b(x-2, y+2);
	auto four  = b(x-3, y+3);

	return (one == two) & (two == three) & (three == four);
}

Board::Player Board::winner() const {
	//---+-------+
	//   |   2   |
	// 1 +-------+
	//   |   3   |
	//---+-------+
	// 0 |   4   |
	//---+-------+
	
	//ONE
	for(size_t x = 0; x < 3; x++) {
		for(size_t y = 3; y < _colHeight[x]; y++) {
			Player p = (*this)(x, y);

			if(checkVertical(*this, x, y)) {
				return p;
			}
		}
	}

	//TWO
	for(size_t x = 3; x < _width; x++) {
		for(size_t y = _height - 3; y < _colHeight[x]; y++) {
			Player p = (*this)(x, y);

			if(checkHorizontal(*this, x, y) ||
			   checkVertical(*this,  x, y)  ||
			   checkUDiag(*this, x, y)) {
				return p;
			}
		}
	}

	//THREE
	for(size_t x = 3; x < _width; x++) {
		for(size_t y = 3; y < _colHeight[x] && y < _height - 3; y++) {
			Player p = (*this)(x, y);

			if(checkHorizontal(*this, x, y) ||
			   checkVertical(*this,  x, y)  ||
			   checkUDiag(*this, x, y) ||
			   checkLDiag(*this, x, y)) {
				return p;
			}
		}
	}

	//FOUR
	for(size_t x = 3; x < _width; x++) {
		for(size_t y = 0; y < _colHeight[x] && y < 3; y++) {
			Player p = (*this)(x, y);

			if(checkHorizontal(*this, x, y) ||
			   checkLDiag(*this, x, y)) {
				return p;
			}
		}
	}

	return Player::E;
}

bool Board::isGameOver() const {
	return isFull() || winner() != Player::E;
}

const std::vector<size_t>& Board::legalMoves() const {
	return _lMovs;
}

std::string Board::toString() const {
	std::stringstream s;

	for(size_t row = _height - 1; row < _height; row--) {
		for(size_t col = 0; col < _width; col++) {
			auto cell = (*this)(col, row);

			s << (cell == Board::Player::E  ? ". " :
			      cell == Board::Player::P1 ? "O " :
			                                  "X ");
		}
		s << '\n';
	}

	for(size_t i = 0; i < 2 * _width - 1; i++) {
		s << '=';
	}
	s << '\n';

	return s.str();
}

bool causedWin(const Board& b, size_t xx, size_t yy) {
	assert(xx < b.width());
	assert(yy < b.height());

	Board::Player p = b(xx, yy);
	if(p == Board::Player::E) {
		return false;
	}
	
	int x = xx;
	int y = yy;
	int width  = b.width();
	int height = b.height();
	
	bool success = false;
	//horizontal
	for(int i = std::max(0, x - 3); i < std::min(width - 3, x + 1); i++) {
		success |= (b(i+0, y) == p) &
		           (b(i+1, y) == p) &
		           (b(i+2, y) == p) &
		           (b(i+3, y) == p);
	}
	
	//vertical
	if(y >= 3) {
		success |= (b(x, y-0) == p) &
			   (b(x, y-1) == p) &
			   (b(x, y-2) == p) &
			   (b(x, y-3) == p);
	}
	
	if(success) {
		return true;
	}

	//upper diagonal
	for(int i = std::max(-3, -std::min(x, y)); i < std::min(1, std::min(width-x, height-y) -3); i++) {
		success |= (b(x+i+0, y+i+0) == p) &
		           (b(x+i+1, y+i+1) == p) &
		           (b(x+i+2, y+i+2) == p) &
		           (b(x+i+3, y+i+3) == p);
	}

	if(success) { 
		return true;
	}
	
	//lower diagonal
	for(int i = std::max(-3, -std::min(x, height-y-1)); i < std::min(1, std::min(width-x, y+1) -3); i++) {
		success |= (b(x+i+0, y-i-0) == p) &
		           (b(x+i+1, y-i-1) == p) &
		           (b(x+i+2, y-i-2) == p) &
		           (b(x+i+3, y-i-3) == p);
	}
	
	return success;
}

