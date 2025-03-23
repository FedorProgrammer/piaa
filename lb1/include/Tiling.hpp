#ifndef TILING_HPP_
#define TILING_HPP_

#include <utility>
#include <vector>

#include "Board.hpp"

namespace tiling {

std::pair<Board, std::vector<Board>> backtrack(Board startBoard, bool record);
std::pair<Board, std::vector<Board>> solve(int size, bool record);

}  // namespace tiling
#endif  // TILING_HPP_