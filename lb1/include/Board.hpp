#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <utility>
#include <vector>

#include "Square.hpp"

class Board {
 private:
  int size;
  int emptyCells;
  int startX, startY;
  std::vector<Square> squares;

  bool isPointOccupied(int x, int y) const;

 public:
  Board(int size);
  Board(const Board& other);
  Board& operator=(const Board& other);

  int getSize() const { return size; }
  int getEmptyCells() const { return emptyCells; }
  const std::vector<Square>& getSquares() const { return squares; }
  int getSquaresCount() const { return squares.size(); }

  void setStartCoordinates(int x, int y) {
    startX = x;
    startY = y;
  }

  bool isValid(std::pair<int, int> coordinates, int squareSize) const;

  void addSquare(const Square& square);

  std::pair<int, int> findEmptyCell() const;

  void scale(int scaleFactor);
};

#endif  // BOARD_HPP_