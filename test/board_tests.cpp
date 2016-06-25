#include "Board.hpp"
#include <gtest/gtest.h>

#include <vector>

TEST(BoardTest, Initialization) {
	Board b(7, 6);
	
	ASSERT_EQ(b.width(), 7);
	ASSERT_EQ(b.height(), 6);

	for(size_t i = 0; i < b.width(); i++) {
		for(size_t j = 0; j < b.height(); j++) {
			EXPECT_EQ(b(i, j), Board::Player::E) << "Cell at " << i << " " << j << " is not empty";
		}
	}
}

TEST(BoardTest, Reset) {
	Board b(7, 6);
	for(size_t i = 0; i < b.width(); i++) {
		for(size_t j = 0; j < b.height(); j++) {
			b.put(Board::Player::P1, i);
		}
	}

	b.reset();
	EXPECT_EQ(b.width(), 7);
	EXPECT_EQ(b.height(), 6);

	for(size_t i = 0; i < b.width(); i++) {
		for(size_t j = 0; j < b.height(); j++) {
			EXPECT_EQ(b(i, j), Board::Player::E);
		}
	}
}

TEST(BoardTest, Put) {
	Board b(7, 6);
	
	for(size_t i = 0; i < b.width(); i++) {
		EXPECT_EQ(b.put(Board::Player::P1, i), 0);
		EXPECT_EQ(b.put(Board::Player::E, i), b.height());
		EXPECT_EQ(b.put(Board::Player::P2, i), 1);
	}

	for(size_t i = 0; i < b.width(); i++) {
		EXPECT_EQ(b(i, 0), Board::Player::P1);
		EXPECT_EQ(b(i, 1), Board::Player::P2);
	}
}

TEST(BoardTest, Overflow) {
	Board b(7, 6);

	for(size_t i = 0; i < b.width(); i++) {
		for(size_t j = 0; j < 3 * b.height(); j++) {
			EXPECT_LE(b.put(Board::Player::P1, i), b.height());
		}
		EXPECT_TRUE(b.isColumnFull(i));
		EXPECT_EQ(b.put(Board::Player::P1, i), b.height());
	}

	EXPECT_TRUE(b.isFull());
}

TEST(BoardTest, CopyConstructors) {
	Board b(7, 6);
	
	for(size_t i = 0; i < b.width(); i++) {
		b.put(Board::Player::P1, i);
		b.put(Board::Player::P2, i);
		b.put(Board::Player::P2, i);
	}

	//copy constructor
	Board b2(b);
	ASSERT_EQ(b.width(), b2.width());
	ASSERT_EQ(b.height(), b2.height());

	for(size_t i = 0; i < b.width(); i++) {
		for(size_t j = 0; j < b.height(); j++) {
			EXPECT_EQ(b(i, j), b2(i, j));
		}
	}
	
	//copy assignment
	b2.reset();
	b2 = b;
	
	ASSERT_EQ(b.width(), b2.width());
	ASSERT_EQ(b.height(), b2.height());

	for(size_t i = 0; i < b.width(); i++) {
		for(size_t j = 0; j < b.height(); j++) {
			EXPECT_EQ(b(i, j), b2(i, j));
		}
	}
}

TEST(BoardTest, HorizontalWinLeftToRight) {
	Board b(7, 6);
	
	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	for(size_t i = 0; i < 3; i++) {
		b.put(Board::Player::P1, i);
		EXPECT_FALSE(causedWin(b, i, 0));
	}

	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	b.put(Board::Player::P1, 3);
	EXPECT_TRUE(causedWin(b, 3, 0));

	EXPECT_EQ(b.winner(), Board::Player::P1);
	EXPECT_TRUE(b.isGameOver());
}

TEST(BoardTest, HorizontalWinRightToLeft) {
	Board b(7, 6);
	
	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	for(size_t i = 3; i > 0; i--) {
		b.put(Board::Player::P1, i);
		EXPECT_FALSE(causedWin(b, i, 0));
	}

	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	b.put(Board::Player::P1, 0);
	EXPECT_TRUE(causedWin(b, 0, 0));

	EXPECT_EQ(b.winner(), Board::Player::P1);
	EXPECT_TRUE(b.isGameOver());
}

TEST(BoardTest, HorizontalWinMiddle) {
	Board b(7, 6);
	
	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	// 1 1 . 1
	b.put(Board::Player::P1, 0);
	EXPECT_FALSE(causedWin(b, 0, 0));
	b.put(Board::Player::P1, 1);
	EXPECT_FALSE(causedWin(b, 1, 0));
	b.put(Board::Player::P1, 3);
	EXPECT_FALSE(causedWin(b, 3, 0));

	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	b.put(Board::Player::P1, 2);
	EXPECT_TRUE(causedWin(b, 2, 0));

	EXPECT_EQ(b.winner(), Board::Player::P1);
	EXPECT_TRUE(b.isGameOver());
}

TEST(BoardTest, VerticalWin) {
	Board b(7, 6);
	
	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	for(size_t i = 0; i < 3; i++) {
		b.put(Board::Player::P2, 0);
		EXPECT_FALSE(causedWin(b, 0, i));
	}

	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	b.put(Board::Player::P2, 0);
	EXPECT_TRUE(causedWin(b, 0, 3));

	EXPECT_EQ(b.winner(), Board::Player::P2);
	EXPECT_TRUE(b.isGameOver());
}

TEST(BoardTest, VerticalWinTop) {
	Board b(7, 6);
	
	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());
	
	for(size_t i = 0; i < 2; i++) {
		b.put(Board::Player::P1, 0);
		EXPECT_FALSE(causedWin(b, 0, i));
	}

	for(size_t i = 0; i < 3; i++) {
		b.put(Board::Player::P2, 0);
		EXPECT_FALSE(causedWin(b, 0, i+2));
	}

	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	b.put(Board::Player::P2, 0);
	EXPECT_TRUE(causedWin(b, 0, 5));

	EXPECT_EQ(b.winner(), Board::Player::P2);
	EXPECT_TRUE(b.isGameOver());
}

TEST(BoardTest, UpperDiagonalWinTop) {
	Board b(7, 6);

	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	// . . . .
	// . . 1 2
	// . 1 2 2
	// 1 2 2 2
	b.put(Board::Player::P1, 0);
	b.put(Board::Player::P2, 1);
	b.put(Board::Player::P1, 1);
	b.put(Board::Player::P2, 2);
	b.put(Board::Player::P2, 2);
	b.put(Board::Player::P1, 2);
	b.put(Board::Player::P2, 3);
	b.put(Board::Player::P2, 3);
	b.put(Board::Player::P2, 3);
	
	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());
	
	b.put(Board::Player::P1, 3);
	EXPECT_TRUE(causedWin(b, 3, 3));
	
	EXPECT_EQ(b.winner(), Board::Player::P1);
	EXPECT_TRUE(b.isGameOver());
}

TEST(BoardTest, UpperDiagonalWinBottom) {
	Board b(7, 6);

	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	// . . . 1
	// . . 1 2
	// . 1 2 2
	// . 2 2 2
	b.put(Board::Player::P2, 1);
	b.put(Board::Player::P1, 1);
	b.put(Board::Player::P2, 2);
	b.put(Board::Player::P2, 2);
	b.put(Board::Player::P1, 2);
	b.put(Board::Player::P2, 3);
	b.put(Board::Player::P2, 3);
	b.put(Board::Player::P2, 3);
	b.put(Board::Player::P1, 3);
	
	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());
	
	b.put(Board::Player::P1, 0);
	EXPECT_TRUE(causedWin(b, 0, 0));
	
	EXPECT_EQ(b.winner(), Board::Player::P1);
	EXPECT_TRUE(b.isGameOver());
}

TEST(BoardTest, UpperDiagonalWinMiddle) {
	Board b(7, 6);

	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	// . . . 1
	// . . 1 2
	// . . 2 2
	// 1 2 2 2
	b.put(Board::Player::P1, 0);
	b.put(Board::Player::P2, 1);
	b.put(Board::Player::P2, 2);
	b.put(Board::Player::P2, 2);
	b.put(Board::Player::P1, 2);
	b.put(Board::Player::P2, 3);
	b.put(Board::Player::P2, 3);
	b.put(Board::Player::P2, 3);
	b.put(Board::Player::P1, 3);
	
	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());
	
	b.put(Board::Player::P1, 1);
	EXPECT_TRUE(causedWin(b, 1, 1));
	
	EXPECT_EQ(b.winner(), Board::Player::P1);
	EXPECT_TRUE(b.isGameOver());
}

TEST(BoardTest, LowerDiagonalWinTop) {
	Board b(7, 6);

	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	// . . . .
	// 1 2 . .
	// 1 1 2 .
	// 1 1 1 2
	b.put(Board::Player::P1, 0);
	b.put(Board::Player::P1, 0);
	b.put(Board::Player::P1, 0);
	b.put(Board::Player::P1, 1);
	b.put(Board::Player::P1, 1);
	b.put(Board::Player::P2, 1);
	b.put(Board::Player::P1, 2);
	b.put(Board::Player::P2, 2);
	b.put(Board::Player::P2, 3);
	
	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());
	
	b.put(Board::Player::P2, 0);
	EXPECT_TRUE(causedWin(b, 0, 3));
	
	EXPECT_EQ(b.winner(), Board::Player::P2);
	EXPECT_TRUE(b.isGameOver());
}

TEST(BoardTest, LowerDiagonalWinBottom) {
	Board b(7, 6);

	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	// 2 . . .
	// 1 2 . .
	// 1 1 2 .
	// 1 1 1 .
	b.put(Board::Player::P1, 0);
	b.put(Board::Player::P1, 0);
	b.put(Board::Player::P1, 0);
	b.put(Board::Player::P2, 0);
	b.put(Board::Player::P1, 1);
	b.put(Board::Player::P1, 1);
	b.put(Board::Player::P2, 1);
	b.put(Board::Player::P1, 2);
	b.put(Board::Player::P2, 2);
	
	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());
	
	b.put(Board::Player::P2, 3);
	EXPECT_TRUE(causedWin(b, 3, 0));
	
	EXPECT_EQ(b.winner(), Board::Player::P2);
	EXPECT_TRUE(b.isGameOver());
}

TEST(BoardTest, LowerDiagonalWinMiddle) {
	Board b(7, 6);

	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	// 2 . . .
	// 1 . . .
	// 1 1 2 .
	// 1 1 1 2
	b.put(Board::Player::P1, 0);
	b.put(Board::Player::P1, 0);
	b.put(Board::Player::P1, 0);
	b.put(Board::Player::P2, 0);
	b.put(Board::Player::P1, 1);
	b.put(Board::Player::P1, 1);
	b.put(Board::Player::P1, 2);
	b.put(Board::Player::P2, 2);
	b.put(Board::Player::P2, 3);
	
	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());
	
	b.put(Board::Player::P2, 1);
	EXPECT_TRUE(causedWin(b, 1, 2));
	
	EXPECT_EQ(b.winner(), Board::Player::P2);
	EXPECT_TRUE(b.isGameOver());
}

TEST(BoardTest, NoWin) {
	Board b(7, 6);

	EXPECT_EQ(b.winner(), Board::Player::E);
	EXPECT_FALSE(b.isGameOver());

	for(size_t i = 0; i < b.width(); i++) {
		for(size_t j = 0; j < b.height(); j++) {
			EXPECT_FALSE(causedWin(b, i, j));
		}
	}
}

TEST(BoardTest, LegalMoves) {
	Board b(5, 1);
	std::vector<size_t> expected;

	expected = { 0, 1, 2, 3, 4 };
	EXPECT_EQ(b.legalMoves(), expected);

	b.put(Board::Player::P1, 0);
	expected = { 1, 2, 3, 4};
	EXPECT_EQ(b.legalMoves(), expected);

	b.put(Board::Player::P1, 3);
	expected = { 1, 2, 4 };
	EXPECT_EQ(b.legalMoves(), expected);
	
	b.put(Board::Player::P1, 4);
	expected = { 1, 2 };
	EXPECT_EQ(b.legalMoves(), expected);
	
	b.put(Board::Player::P1, 1);
	b.put(Board::Player::P1, 2);
	expected = { };
	EXPECT_EQ(b.legalMoves(), expected);
}
