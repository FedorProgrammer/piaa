#include "../include/Tiling.hpp"

#include <cmath>
#include <cstdint>
#include <set>
#include <stack>

#include "../include/Board.hpp"
#include "../include/Square.hpp"

namespace tiling {

namespace details {

std::set<int> baseFactorize(int n) {
  std::set<int> factors;
  for (int i = 2; i <= std::sqrt(n); i++) {
    if (n % i == 0) {
      factors.insert(i);
      while (n % i == 0) {
        n /= i;
      }
    }
  }
  if (n > 1) {
    factors.insert(n);
  }
  return factors;
}

Board initPrimeBoard(int size) {
  Board initBoard(size);

  int centralSize = (size + 1) / 2;
  int sideSize = size / 2;

  Square central({0, 0}, centralSize);
  Square rightSide({centralSize, 0}, sideSize);
  Square leftSide({0, centralSize}, sideSize);

  initBoard.addSquare(central);
  initBoard.addSquare(rightSide);
  initBoard.addSquare(leftSide);
  initBoard.setStartCoordinates(centralSize, sideSize);

  return initBoard;
}

}  // namespace details

std::pair<Board, std::vector<Board>> backtrack(Board startBoard, bool record) {
  int minimalCount = INT32_MAX;
  Board bestBoard(startBoard.getSize());

  std::stack<Board> stack;
  stack.push(startBoard);

  std::vector<Board> steps;

  while (!stack.empty()) {
    Board board = stack.top();
    stack.pop();
    if (record) steps.push_back(board);

    if (board.getSquaresCount() >= minimalCount) {
      continue;
    } else if (board.getEmptyCells() == 0) {
      minimalCount = board.getSquaresCount();
      bestBoard = board;
      continue;
    }

    auto [x, y] = board.findEmptyCell();

    for (int squareSize = 1;
         squareSize <= std::min(board.getSize() - x, board.getSize() - y);
         ++squareSize) {
      if (board.isValid({x, y}, squareSize)) {
        Board newBoard = board;
        Square square({x, y}, squareSize);
        newBoard.addSquare(square);

        if (x + squareSize < board.getSize()) {
          newBoard.setStartCoordinates(x + squareSize, y);
        } else {
          newBoard.setStartCoordinates(0, y + 1);
        }

        stack.push(newBoard);
        if (record) steps.push_back(newBoard);
      }
    }
    steps.pop_back();
  }

  return {bestBoard, steps};
}

std::pair<Board, std::vector<Board>> solve(int size, bool record) {
  std::set<int> factors = details::baseFactorize(size);
  int minimal = *factors.begin();
  int coef = size / minimal;

  Board startBoard = details::initPrimeBoard(minimal);
  auto [board, steps] = backtrack(startBoard, record);
  board.scale(coef);

  if (record) {
    for (Board& step : steps) {
      step.scale(coef);
    }
  }

  return {board, steps};
}

}  // namespace tiling