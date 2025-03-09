#ifndef TILING_HPP_
#define TILING_HPP_

#include <set>

#include "Board.hpp"

std::set<int> baseFactorize(int n);
Board backtrack(Board startBoard);
Board solve(int size);

#endif  // TILING_HPP_