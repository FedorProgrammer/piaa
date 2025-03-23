#include "../include/Benchmark.hpp"

#include <chrono>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../include/JsonHandler.hpp"
#include "../include/Serializer.hpp"
#include "../include/Tiling.hpp"

Benchmark::Benchmark(int iterations) : iterations(iterations) {}

void Benchmark::run(size_t minSize, size_t maxSize) {
  benchmarkResults.clear();

  for (size_t n = minSize; n < maxSize + 1; n++) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Testing board size " << n << "..." << std::endl;

    double totalTime = 0.0;
    Board result(n);

    for (int i = 0; i < iterations; i++) {
      auto start = std::chrono::high_resolution_clock::now();
      result = tiling::solve(n, false).first;
      auto end = std::chrono::high_resolution_clock::now();

      std::chrono::duration<double> elapsed = end - start;
      totalTime += elapsed.count();

      std::cout << "  Iteration " << (i + 1) << "/" << iterations << ": "
                << elapsed.count() << "s" << std::endl;
    }

    double averageTime = totalTime / iterations;
    std::cout << "Average time for board size " << n << ": " << averageTime
              << " seconds" << std::endl;

    benchmarkResults.emplace_back(averageTime, result);

    std::cout << "----------------------------------------" << std::endl;
  }
}

void Benchmark::save(const std::string& output) const {
  nlohmann::json results = nlohmann::json::object();
  nlohmann::json benchmarks = nlohmann::json::array();

  for (const auto& result : benchmarkResults) {
    nlohmann::json entry = nlohmann::json::object();
    entry["time"] = result.time;
    entry["board"] = serialize::to_json(result.board);
    benchmarks.push_back(entry);
  }
  results["benchmarks"] = benchmarks;
  results["iterations"] = iterations;

  JsonHandler handler(output);
  handler.write(results);

  std::cout << "\nResults saved to " << output << std::endl;
}
