#include "../include/Board.hpp"

Board::Board(int size) : size(size), emptyCells(size * size) {}

Board::Board(const Board& other)
    : size(other.size),
      emptyCells(other.emptyCells),
      startX(other.startX),
      startY(other.startY),
      squares(other.squares) {}

Board& Board::operator=(const Board& other) {
  if (this != &other) {
    size = other.size;
    emptyCells = other.emptyCells;
    startX = other.startX;
    startY = other.startY;
    squares = other.squares;
  }
  return *this;
}

bool Board::isPointOccupied(int x, int y) const {
  for (const Square& square : squares) {
    if (x >= square.x && x < square.x + square.size && y >= square.y &&
        y < square.y + square.size) {
      return true;
    }
  }
  return false;
}

bool Board::isValid(std::pair<int, int> coordinates, int squareSize) const {
  if (squareSize >= size) {
    return false;
  }

  int x = coordinates.first;
  int y = coordinates.second;

  if (x < 0 || y < 0 || x + squareSize > size || y + squareSize > size) {
    return false;
  }

  for (const Square& square : squares) {
    if (x < square.x + square.size && x + squareSize > square.x &&
        y < square.y + square.size && y + squareSize > square.y) {
      return false;
    }
  }
  return true;
}

void Board::addSquare(const Square& square) {
  squares.push_back(square);
  emptyCells -= square.size * square.size;
}

std::pair<int, int> Board::findEmptyCell() const {
  for (int y = startY; y < size; y++) {
    int startX_pos = (y == startY ? startX : 0);
    for (int x = startX_pos; x < size; x++) {
      if (!isPointOccupied(x, y)) {
        return {x, y};
      }
    }
  }

  for (int y = 0; y < startY; y++) {
    for (int x = 0; x < size; x++) {
      if (!isPointOccupied(x, y)) {
        return {x, y};
      }
    }
  }

  return {-1, -1};
}

void Board::scale(int scaleFactor) {
  for (Square& square : squares) {
    square.x *= scaleFactor;
    square.y *= scaleFactor;
    square.size *= scaleFactor;
  }
  size *= scaleFactor;
}