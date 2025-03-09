#ifndef SQUARE_HPP_
#define SQUARE_HPP_

#include <utility>

struct Square {
  int x, y;
  int size;

  Square() = default;
  Square(std::pair<int, int> coordinates, int size)
      : x(coordinates.first), y(coordinates.second), size(size) {}
};

#endif  // SQUARE_HPP_