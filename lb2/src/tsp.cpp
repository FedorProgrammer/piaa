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
    const std::vector<piece_t>& pieces) {
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

  return arcs;
}

std::vector<piece_t> getPieces(const std::vector<int>& path,
                               const std::unordered_set<int>& remaining) {
  std::vector<piece_t> pieces;
  if (!path.empty()) {
    pieces.push_back({path.front(), path.back()});
  }

  for (int v : remaining) {
    pieces.push_back({v, v});
  }

  return pieces;
}

double getHalfSumBound(const std::vector<std::vector<double>>& weights,
                       const std::vector<piece_t>& pieces) {
  if (pieces.size() <= 1) return 0;

  double summ = 0;
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
  }

  return summ / 2;
}

double getMSTBound(const std::vector<std::vector<double>>& weights,
                   const std::vector<piece_t>& pieces) {
  if (pieces.size() <= 1) return 0;

  size_t verticesCount = pieces.size();
  if (pieces.front().first != pieces.front().second) {
    verticesCount++;
  }

  std::unordered_set<int> vertices;
  std::vector<std::tuple<double, int, int>> arcs = getArcs(weights, pieces);

  double summ = 0;
  for (const auto& arc : arcs) {
    if (vertices.size() >= verticesCount) break;

    auto [weight, from, to] = arc;
    if ((vertices.count(from) != vertices.count(to))) {
      vertices.insert(from);
      vertices.insert(to);
      summ += weight;
    }
  }

  return summ;
}

double getLowerBound(const std::vector<std::vector<double>> weights,
                     const std::vector<piece_t>& pieces) {
  return std::max(getHalfSumBound(weights, pieces),
                  getMSTBound(weights, pieces));
}

void search(const std::vector<std::vector<double>>& weights,
            std::vector<int>& path, std::unordered_set<int>& remaining,
            double currentWeight, double& bestWeight,
            std::vector<int>& bestPath) {
  if (remaining.empty()) {
    double total = currentWeight + weights[path.back()][path.front()];
    if (total < bestWeight || (total == bestWeight && path < bestPath)) {
      bestPath = path;
      bestWeight = total;
    }
    return;
  }

  std::vector<std::pair<int, int>> candidates;

  std::vector<int> rem{remaining.begin(), remaining.end()};
  for (int v : rem) {
    path.push_back(v);
    remaining.erase(v);

    std::vector<piece_t> pieces = getPieces(path, remaining);
    double boundary = getLowerBound(weights, pieces);

    remaining.insert(v);
    path.pop_back();

    candidates.push_back({weights[path.back()][v] + boundary, v});
  }

  std::sort(candidates.begin(), candidates.end());

  for (auto& c : candidates) {
    if (currentWeight + c.first > bestWeight) continue;

    int v = c.second;
    double edgeCost = weights[path.back()][v];

    path.push_back(v);
    remaining.erase(v);

    search(weights, path, remaining, currentWeight + edgeCost, bestWeight,
           bestPath);

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

  search(weights, path, remaining, 0.0, bestWeight, bestPath);

  return {bestPath, bestWeight};
}

std::pair<std::vector<int>, double> amrSearch(
    const std::vector<std::vector<double>>& weights, bool record) {
  size_t N = weights.size();

  std::vector<int> bestPath;
  for (size_t i = 0; i < N; i++) bestPath.push_back(i);
  double bestCost = getCost(weights, bestPath);

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
      int city_to_move = bestPath[from];

      double bestImp = 0;
      size_t transition = from;

      for (size_t to = 1; to < N; to++) {
        if (to == from || to == (from + 1) % N) continue;

        std::vector<int> path = bestPath;
        path.erase(path.begin() + from);

        size_t insert_pos = to;
        if (to > from) insert_pos--;

        path.insert(path.begin() + insert_pos, city_to_move);

        double new_cost = getCost(weights, path);
        double improvement = bestCost - new_cost;

        if (improvement > 0 && improvement > bestImp) {
          bestImp = improvement;
          transition = to;
        }
      }

      if (bestImp > 0) {
        bestPath.erase(bestPath.begin() + from);

        size_t insert_pos = transition;
        if (transition > from) insert_pos--;

        bestPath.insert(bestPath.begin() + insert_pos, city_to_move);
        bestCost -= bestImp;

        m = true;
        count++;
        break;
      }
    }
  }

  return {bestPath, bestCost};
}

}  // namespace tsp