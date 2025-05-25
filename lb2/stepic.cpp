#include <algorithm>
#include <cfloat>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>

using piece_t = std::pair<int, int>;

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

std::pair<std::vector<int>, double> bnbSearch(
    const std::vector<std::vector<double>>& weights) {
  int n = weights.size();
  double bestWeight = DBL_MAX;
  std::vector<int> bestPath;

  std::vector<int> path = {0};
  std::unordered_set<int> remaining;
  for (int i = 1; i < n; i++) remaining.insert(i);

  search(weights, path, remaining, 0.0, bestWeight, bestPath);

  return {bestPath, bestWeight};
}

std::vector<std::vector<double>> inputMatrix(int n) {
  std::vector<std::vector<double>> matrix(n, std::vector<double>(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cin >> matrix[i][j];
    }
  }
  return matrix;
}

int main() {
  int n;
  std::cin >> n;

  std::vector<std::vector<double>> weights = inputMatrix(n);

  auto [path, weight] = bnbSearch(weights);

  for (size_t i = 0; i < path.size(); i++) {
    if (i < path.size() - 1) {
      std::cout << path[i] << ' ';
    } else {
      std::cout << path[i] << std::endl;
    }
  }

  std::cout << std::fixed << std::setprecision(1) << weight << std::endl;

  return 0;
}