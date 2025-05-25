#ifndef TSP_HPP_
#define TSP_HPP_

#include <utility>
#include <vector>

namespace tsp {

using piece_t = std::pair<int, int>;
  
std::pair<std::vector<int>, double> bnbSearch(
    const std::vector<std::vector<double>>& matrix, bool record);

std::pair<std::vector<int>, double> amrSearch(
    const std::vector<std::vector<double>>& matrix, bool record);

}  // namespace tsp

#endif  // TSP_HPP_