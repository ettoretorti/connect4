#include "Game.hpp"
#include "Board.hpp"
#include "TermPlayer.hpp"
#include "RandomPlayer.hpp"
#include "MonteCarloPlayer.hpp"
#include "HybridPlayer.hpp"

#include <iostream>
#include <memory>

int main() {
	Player* p1 = new HybridPlayer(8000, 5);
	Player* p2 = new MonteCarloPlayer(8000);

	Game g(Board(7, 6), std::unique_ptr<Player>(p1), std::unique_ptr<Player>(p2));

	while(!g.step());
	
	std::cout << g.board().toString() << std::endl;
	std::cout << "THE WINNER IS: " << (unsigned int)g.board().winner() << std::endl;

	return 0;
}
