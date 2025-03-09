#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

#include "../include/Board.hpp"
#include "../include/Square.hpp"
#include "../include/Tiling.hpp"

#define _DEFAULT_ITERATIONS 3
#define _DEFAULT_OUTPUT_FILE "results.json"

#define _MIN_SIZE 2
#define _MAX_SIZE 40

struct BenchmarkResult {
  int size;
  double time;
  int iterations;
  Board board;

  BenchmarkResult(int size, double time, int iterations, Board result)
      : size(size), time(time), iterations(iterations), board(result) {}
};

class Benchmark {
 private:
  int iterations;
  std::vector<BenchmarkResult> benchmarkResults;

 public:
  Benchmark(int iterations) : iterations(iterations) {}

  void runBenchmarks(const std::vector<int>& board_sizes) {
    benchmarkResults.clear();

    for (size_t idx = 0; idx < board_sizes.size(); idx++) {
      int n = board_sizes[idx];
      std::cout << "----------------------------------------" << std::endl;
      std::cout << "Testing board size " << n << "..." << std::endl;

      double totalTime = 0.0;
      Board result(n);

      for (int i = 0; i < iterations; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        result = solve(n);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        totalTime += elapsed.count();

        std::cout << "  Iteration " << (i + 1) << "/" << iterations << ": "
                  << elapsed.count() << "s" << std::endl;
      }

      double averageTime = totalTime / iterations;
      std::cout << "Average time for board size " << n << ": " << averageTime
                << " seconds" << std::endl;

      benchmarkResults.emplace_back(n, averageTime, iterations, result);

      std::cout << "----------------------------------------" << std::endl;
    }
  }

  void saveResultsToJson(const std::string& output_file) const {
    nlohmann::json results = {};
    results["benchmarks"] = nlohmann::json::array();

    for (const auto& result : benchmarkResults) {
      nlohmann::json entry = {};
      entry["size"] = result.size;
      entry["time"] = result.time;
      entry["iterations"] = result.iterations;

      entry["squares"] = nlohmann::json::array();
      std::vector<Square> squares = result.board.getSquares();
      for (size_t i = 0; i < squares.size(); i++) {
        Square square = squares[i];
        nlohmann::json info = {};

        info["size"] = square.size;
        info["x"] = square.x;
        info["y"] = square.y;
        entry["squares"].push_back(info);
      }

      results["benchmarks"].push_back(entry);
    }

    std::ofstream outputFile(output_file);
    outputFile << std::setw(2) << results << std::endl;
    outputFile.close();

    std::cout << "\nResults saved to " << output_file << std::endl;
  }

  const std::vector<BenchmarkResult>& getResults() const {
    return benchmarkResults;
  }
};

int main() {
  std::vector<int> board_sizes;
  for (int i = _MIN_SIZE; i <= _MAX_SIZE; i++) {
    board_sizes.push_back(i);
  }

  Benchmark benchmark(_DEFAULT_ITERATIONS);

  benchmark.runBenchmarks(board_sizes);
  benchmark.saveResultsToJson(_DEFAULT_OUTPUT_FILE);

  return 0;
}