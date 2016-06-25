#include "TermPlayer.hpp"
#include "Board.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

size_t TermPlayer::makeMove(const Board& b, Board::Player p) {
	using namespace std;
	
	if(p == Board::Player::E) {
		throw invalid_argument("E passed as player");
	}

	auto str = b.toString();
	cout << str << endl;

	cout << "Please insert a move for player " << (p == Board::Player::P1 ? "O" : "X") << ". (First column is 0)" << endl;
	
	size_t input = -1;	
	string line;
retry:
	getline(cin, line);
	try {
		input = stoul(line);
	} catch(...) {
		cout << "Move must be a valid number." << endl;
		goto retry;
	}
	
	if(input > b.width() || b.isColumnFull(input)) {
		cout << "Invalid move." << endl;
		goto retry;
	}

	return input;
}
