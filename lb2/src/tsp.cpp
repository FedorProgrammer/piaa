#include "tsp.hpp"

#include <algorithm>
#include <cfloat>
#include <climits>
#include <cstddef>
#include <iostream>
#include <unordered_set>

namespace tsp {

namespace {

double getCost(const std::vector<std::vector<double>>& weights,
               const std::vector<int>& path) {
  double summ = 0;
  for (size_t i = 0; i < path.size(); i++) {
    summ += weights[path[i]][path[(i + 1) % path.size()]];
  }
  return summ;
}

std::vector<std::tuple<double, int, int>> getArcs(
    const std::vector<std::vector<double>>& weights,
    const std::vector<piece_t>& pieces, bool record) {
  std::vector<std::tuple<double, int, int>> arcs;

  for (size_t i = 0; i < pieces.size(); i++) {
    for (size_t j = 0; j < pieces.size(); j++) {
      if (i == j) continue;

      int from = pieces[i].second;
      int to = pieces[j].first;
      double weight = weights[from][to];

      arcs.push_back({weight, from, to});
    }
  }

  std::sort(arcs.begin(), arcs.end());

  if (record) {
    std::cout << std::endl;
    std::cout << "--------------------------------" << std::endl;

    std::cout << "[getArcs]" << std::endl;
    std::cout << "pieces count: [" << pieces.size() << "]" << std::endl;
    std::cout << "arcs found: [" << arcs.size() << "]" << std::endl;
    std::cout << "sorted arcs (weight, from, to): [" << std::endl;
    for (size_t i = 0; i < arcs.size(); i++) {
      auto [w, from, to] = arcs[i];
      std::cout << "  (" << w << ", " << from << ", " << to << ")" << std::endl;
    }
    std::cout << "]" << std::endl;

    std::cout << "--------------------------------" << std::endl;
    std::cout << std::endl;
  }

  return arcs;
}

std::vector<piece_t> getPieces(const std::vector<int>& path,
                               const std::unordered_set<int>& remaining,
                               bool record) {
  std::vector<piece_t> pieces;
  if (!path.empty()) {
    pieces.push_back({path.front(), path.back()});
  }

  for (int v : remaining) {
    pieces.push_back({v, v});
  }

  if (record) {
    std::cout << std::endl;
    std::cout << "--------------------------------" << std::endl;

    std::cout << "[getPieces]" << std::endl;
    std::cout << "pieces (first, second): [" << std::endl;
    for (const auto& p : pieces) {
      std::cout << "  (" << p.first << ", " << p.second << ")" << std::endl;
    }
    std::cout << "]" << std::endl;

    std::cout << "--------------------------------" << std::endl;
    std::cout << std::endl;
  }

  return pieces;
}

double getHalfSumBound(const std::vector<std::vector<double>>& weights,
                       const std::vector<piece_t>& pieces, bool record) {
  if (pieces.size() <= 1) return 0;

  double summ = 0;

  if (record) {
    std::cout << std::endl;
    std::cout << "--------------------------------" << std::endl;

    std::cout << "[getHalfSumBound] enter" << std::endl;
    std::cout << "pieces count: [" << pieces.size() << "]" << std::endl;
  }

  for (size_t i = 0; i < pieces.size(); i++) {
    double enter = DBL_MAX;
    double exit = DBL_MAX;

    for (size_t j = 0; j < pieces.size(); j++) {
      if (i == j) continue;

      double arcIn = weights[pieces[j].second][pieces[i].first];
      double arcOut = weights[pieces[i].second][pieces[j].first];

      enter = std::min(enter, arcIn);
      exit = std::min(exit, arcOut);
    }

    summ += enter + exit;

    if (record) {
      std::cout << std::endl;
      std::cout << "--------------------------------" << std::endl;

      std::cout << "[getHalfSumBound] iteration" << std::endl;
      std::cout << "piece (" << pieces[i].first << ", " << pieces[i].second
                << ')' << std::endl;
      std::cout << "min_enter = [" << enter << ']' << std::endl;
      std::cout << "min_exit = [" << exit << ']' << std::endl;

      std::cout << "--------------------------------" << std::endl;
      std::cout << std::endl;
    }
  }

  double result = summ / 2;

  if (record) {
    std::cout << std::endl;
    std::cout << "--------------------------------" << std::endl;

    std::cout << "[getHalfSumBound] exit" << std::endl;
    std::cout << "result: [" << result << "]" << std::endl;

    std::cout << "--------------------------------" << std::endl;
    std::cout << std::endl;
  }

  return result;
}

double getMSTBound(const std::vector<std::vector<double>>& weights,
                   const std::vector<piece_t>& pieces, bool record) {
  if (pieces.size() <= 1) return 0;

  size_t verticesCount = pieces.size();
  if (pieces.front().first != pieces.front().second) {
    verticesCount++;
  }

  if (record) {
    std::cout << std::endl;
    std::cout << "--------------------------------" << std::endl;

    std::cout << "[getMSTBound] enter" << std::endl;
    std::cout << "pieces count: [" << pieces.size() << "]" << std::endl;
    std::cout << "vertices count: [" << verticesCount << "]" << std::endl;

    std::cout << "--------------------------------" << std::endl;
    std::cout << std::endl;
  }

  std::unordered_set<int> vertices;
  std::vector<std::tuple<double, int, int>> arcs =
      getArcs(weights, pieces, record);

  double summ = 0;
  for (const auto& arc : arcs) {
    if (vertices.size() >= verticesCount) break;

    auto [weight, from, to] = arc;
    if ((vertices.count(from) != vertices.count(to))) {
      vertices.insert(from);
      vertices.insert(to);
      summ += weight;

      if (record) {
        std::cout << std::endl;
        std::cout << "--------------------------------" << std::endl;

        std::cout << "[getMSTBound] iteration" << std::endl;
        std::cout << "Adding arc: (" << weight << ", " << from << ", " << to
                  << ")" << std::endl;
        std::cout << "summ: [" << summ << ']' << std::endl;

        std::cout << "--------------------------------" << std::endl;
        std::cout << std::endl;
      }
    }
  }

  if (record) {
    std::cout << std::endl;
    std::cout << "--------------------------------" << std::endl;

    std::cout << "[getMSTBound] exit" << std::endl;
    std::cout << "result: [" << summ << "]" << std::endl;

    std::cout << "--------------------------------" << std::endl;
    std::cout << std::endl;
  }

  return summ;
}

double getLowerBound(const std::vector<std::vector<double>> weights,
                     const std::vector<piece_t>& pieces, bool record) {
  double halfSumBound = getHalfSumBound(weights, pieces, record);
  double mstBound = getMSTBound(weights, pieces, record);
  double result = std::max(halfSumBound, mstBound);

  if (record) {
    std::cout << std::endl;
    std::cout << "--------------------------------" << std::endl;

    std::cout << "[getLowerBound]" << std::endl;
    std::cout << "HalfSumBound: [" << halfSumBound << "]" << std::endl;
    std::cout << "MSTBound: [" << mstBound << "]" << std::endl;
    std::cout << "LowerBound: [" << result << "]" << std::endl;

    std::cout << "--------------------------------" << std::endl;
    std::cout << std::endl;
  }

  return result;
}

void search(const std::vector<std::vector<double>>& weights,
            std::vector<int>& path, std::unordered_set<int>& remaining,
            double currentWeight, double& bestWeight,
            std::vector<int>& bestPath, bool record) {
  if (record) {
    std::cout << std::endl;
    std::cout << "--------------------------------" << std::endl;

    std::cout << "[search] enter" << std::endl;
    std::cout << "current path: [";
    for (auto it = path.begin(); it != path.end(); it++) {
      std::cout << *it << (std::next(it) != path.end() ? ' ' : ']');
    }
    std::cout << std::endl;
    std::cout << "current weight: [" << currentWeight << "]" << std::endl;
    std::cout << "remaining vertices: [";
    for (auto it = remaining.begin(); it != remaining.end(); it++) {
      std::cout << *it << (std::next(it) != remaining.end() ? ' ' : ']');
    }
    std::cout << std::endl;

    std::cout << "--------------------------------" << std::endl;
    std::cout << std::endl;
  }

  if (remaining.empty()) {
    double total = currentWeight + weights[path.back()][path.front()];

    if (record) {
      std::cout << std::endl;
      std::cout << "--------------------------------" << std::endl;

      std::cout << "[search] iteration" << std::endl;
      std::cout << "Complete path found! Total cost (with return): [" << total
                << "]" << std::endl;
    }

    if (total < bestWeight || (total == bestWeight && path < bestPath)) {
      if (record) {
        std::cout << "New best path found!" << std::endl;
        std::cout << "Cost: [" << total << "]" << std::endl;
      }

      bestPath = path;
      bestWeight = total;
    }

    if (record) {
      std::cout << "--------------------------------" << std::endl;
      std::cout << std::endl;
    }

    return;
  }

  std::vector<std::pair<int, int>> candidates;

  std::vector<int> rem{remaining.begin(), remaining.end()};
  for (int v : rem) {
    path.push_back(v);
    remaining.erase(v);

    std::vector<piece_t> pieces = getPieces(path, remaining, record);
    double boundary = getLowerBound(weights, pieces, record);

    remaining.insert(v);
    path.pop_back();

    candidates.push_back({weights[path.back()][v] + boundary, v});
  }

  std::sort(candidates.begin(), candidates.end());

  for (auto& c : candidates) {
    if (currentWeight + c.first > bestWeight) {
      if (record) {
        std::cout << std::endl;
        std::cout << "--------------------------------" << std::endl;

        std::cout << "[search] Pruning" << std::endl;
        std::cout << "Skipping vertex [" << c.second << "] - lower bound: ["
                  << currentWeight + c.first << "] > best: [" << bestWeight
                  << "]" << std::endl;

        std::cout << "--------------------------------" << std::endl;
        std::cout << std::endl;
      }

      continue;
    }

    int v = c.second;
    double edgeCost = weights[path.back()][v];

    if (record) {
      std::cout << std::endl;
      std::cout << "--------------------------------" << std::endl;

      std::cout << "[search] Branching" << std::endl;
      std::cout << "Adding vertex [" << v << "] with edge cost: [" << edgeCost
                << "]" << std::endl;

      std::cout << "--------------------------------" << std::endl;
      std::cout << std::endl;
    }

    path.push_back(v);
    remaining.erase(v);

    search(weights, path, remaining, currentWeight + edgeCost, bestWeight,
           bestPath, record);

    path.pop_back();
    remaining.insert(v);
  }
}

}  // namespace

std::pair<std::vector<int>, double> bnbSearch(
    const std::vector<std::vector<double>>& weights, bool record) {
  int n = weights.size();
  double bestWeight = DBL_MAX;
  std::vector<int> bestPath;

  std::vector<int> path = {0};
  std::unordered_set<int> remaining;
  for (int i = 1; i < n; i++) remaining.insert(i);

  if (record) {
    std::cout << std::endl;
    std::cout << "================================" << std::endl;

    std::cout << "[bnbSearch]: enter" << std::endl;
    std::cout << "n (weights.size): [" << n << "]" << std::endl;
    std::cout << "remaining vertices: [";
    for (auto it = remaining.begin(); it != remaining.end(); it++) {
      std::cout << *it << (std::next(it) != remaining.end() ? ' ' : ']');
    }
    std::cout << std::endl;

    std::cout << std::endl;
  }

  search(weights, path, remaining, 0.0, bestWeight, bestPath, record);

  if (record) {
    std::cout << std::endl;

    std::cout << "[bnbSearch]: exit" << std::endl;
    std::cout << "bestWeight: [" << bestWeight << std::endl;
    std::cout << "bestPath: [";
    for (auto it = bestPath.begin(); it != bestPath.end(); it++) {
      std::cout << *it << (std::next(it) != bestPath.end() ? ' ' : ']');
    }
    std::cout << std::endl;

    std::cout << "================================" << std::endl;
    std::cout << std::endl;
  }

  return {bestPath, bestWeight};
}

std::pair<std::vector<int>, double> amrSearch(
    const std::vector<std::vector<double>>& weights, bool record) {
  size_t N = weights.size();

  std::vector<int> bestPath;
  for (size_t i = 0; i < N; i++) bestPath.push_back(i);
  double bestCost = getCost(weights, bestPath);

  if (record) {
    std::cout << std::endl;
    std::cout << "================================" << std::endl;

    std::cout << "[amrSearch]: enter" << std::endl;
    std::cout << "N (weights.size): [" << N << "]" << std::endl;
    std::cout << "initialPath: [";
    for (auto it = bestPath.begin(); it != bestPath.end(); it++) {
      std::cout << *it << (std::next(it) != bestPath.end() ? ' ' : ']');
    }
    std::cout << std::endl;
    std::cout << "initialCost: [" << bestCost << "]" << std::endl;

    std::cout << std::endl;
  }

  int count = 0;
  bool m = true;
  int F = N;

  while (m && count < F) {
    m = false;

    std::vector<std::pair<double, size_t>> potentials;
    for (size_t i = 1; i < N; i++) {
      int prev = bestPath[(i - 1 + N) % N];
      int curr = bestPath[i];
      int next = bestPath[(i + 1) % N];

      double currentСontribution = weights[prev][curr] + weights[curr][next];
      double directСost = weights[prev][next];
      double potential = currentСontribution - directСost;

      potentials.push_back({potential, i});
    }

    std::sort(potentials.begin(), potentials.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    for (const auto& [potential, from] : potentials) {
      int cityToMove = bestPath[from];

      double bestImp = 0;
      size_t transition = from;

      for (size_t to = 1; to < N; to++) {
        if (to == from || to == (from + 1) % N) continue;

        std::vector<int> path = bestPath;
        path.erase(path.begin() + from);

        size_t insertPos = to;
        if (to > from) insertPos--;

        path.insert(path.begin() + insertPos, cityToMove);

        double newCost = getCost(weights, path);
        double improvement = bestCost - newCost;

        if (improvement > 0 && improvement > bestImp) {
          bestImp = improvement;
          transition = to;
        }
      }

      if (record) {
        std::cout << std::endl;
        std::cout << "--------------------------------" << std::endl;

        std::cout << "[amrSearch]: iteration " << count + 1 << " checking city "
                  << cityToMove << std::endl;
        std::cout << "Current position: [" << from << "]" << std::endl;
        std::cout << "Best improvement found: [" << bestImp << "]" << std::endl;
        if (bestImp > 0) {
          std::cout << "Will move to position: ["
                    << (transition > from ? transition - 1 : transition) << "]"
                    << std::endl;
        } else {
          std::cout << "No improvement possible for this city" << std::endl;
        }
        std::cout << "Current path: [";
        for (auto it = bestPath.begin(); it != bestPath.end(); it++) {
          std::cout << *it << (std::next(it) != bestPath.end() ? ' ' : ']');
        }
        std::cout << std::endl;
        std::cout << "Current cost: [" << bestCost << "]" << std::endl;

        std::cout << "--------------------------------" << std::endl;
        std::cout << std::endl;
      }

      if (bestImp > 0) {
        bestPath.erase(bestPath.begin() + from);

        size_t insertPos = transition;
        if (transition > from) insertPos--;

        bestPath.insert(bestPath.begin() + insertPos, cityToMove);
        bestCost -= bestImp;

        m = true;
        count++;
        break;
      }
    }
  }

  if (record) {
    std::cout << std::endl;

    std::cout << "[amrSearch]: exit" << std::endl;
    std::cout << "iterations: [" << count << "]" << std::endl;
    std::cout << "bestCost: [" << bestCost << "]" << std::endl;
    std::cout << "bestPath: [";
    for (auto it = bestPath.begin(); it != bestPath.end(); it++) {
      std::cout << *it << (std::next(it) != bestPath.end() ? ' ' : ']');
    }
    std::cout << std::endl;

    std::cout << "================================" << std::endl;
    std::cout << std::endl;
  }

  return {bestPath, bestCost};
}

}  // namespace tsp