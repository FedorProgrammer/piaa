#include <iostream>

#include "include/Board.hpp"
#include "include/Square.hpp"
#include "include/Tiling.hpp"

#define MIN_SIZE 2
#define MAX_SIZE 40

int main() {
  int n = 0;
  std::cin >> n;

  if (n < MIN_SIZE || n > MAX_SIZE) {
    std::cerr << "Invalid input size!!"<< std::endl;
    return 1;
  }

  Board board = solve(n);
  
  std::cout << board.getSquares().size() << std::endl;
  for (const Square& square : board.getSquares()) {
    std::cout << square.x + 1 << " " << square.y + 1 << " " << square.size
              << std::endl;
  }

  return 0;
}