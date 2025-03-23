#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <string>

#include "../include/Benchmark.hpp"
#include "../include/Board.hpp"
#include "../include/Deserializer.hpp"
#include "../include/JsonHandler.hpp"
#include "../include/Square.hpp"
#include "../include/Tiling.hpp"
#include "../include/Visualizer.hpp"

#define _MIN_SIZE 2
#define _MAX_SIZE 40

#define _JSON_FILE "benchmark/benchmark.json"
#define _ITERATIONS 3

#define _VISUAL_DIR "visual-output/"

void solveMode() {
  int n = 0;
  std::cin >> n;

  if (n < _MIN_SIZE || n > _MAX_SIZE) {
    std::cerr << "Invalid input size (" << _MIN_SIZE << "-" << _MAX_SIZE << ")!"
              << std::endl;
    return;
  }

  Board board = tiling::solve(n, false).first;

  std::cout << board.getSquares().size() << std::endl;
  for (const Square& square : board.getSquares()) {
    std::cout << square.x + 1 << " " << square.y + 1 << " " << square.size
              << std::endl;
  }
}

void benchmarkMode() {
  Benchmark benchmark(_ITERATIONS);
  benchmark.run(_MIN_SIZE, _MAX_SIZE);
  benchmark.save(_JSON_FILE);

  std::cout << "Benchmarks completed and saved to " << _JSON_FILE << std::endl;
}

void visualizeMode() {
  nlohmann::json benchmark = nlohmann::json::object();
  try {
    JsonHandler handler(_JSON_FILE);
    benchmark = handler.read();
  } catch (const std::exception& e) {
    benchmarkMode();
    JsonHandler handler(_JSON_FILE);
    benchmark = handler.read();
  }

  std::vector<int> sizes;
  std::vector<double> times;

  for (const auto& result : benchmark["benchmarks"]) {
    Board board = deserialize::get_board(result["board"]);

    sizes.push_back(board.getSize());
    times.push_back(result["time"].get<double>());

    std::string title =
        "Замощение квадрата со стороной: " + std::to_string(board.getSize()) +
        "x" + std::to_string(board.getSize());
    std::string filename = "tiling_" + std::to_string(board.getSize()) + ".png";

    std::string path =
        visualize::visualizeTiling(board, title, filename, _VISUAL_DIR);
    std::cout << "Generated: " << path << std::endl;
  }

  std::string path = visualize::visualizeExecutionTimes(
      sizes, times, "execution_times.png", _VISUAL_DIR);
  std::cout << "Generated execution time graph: " << path << std::endl;
}

void stepsMode() {
  int n = 0;
  std::cin >> n;

  if (n < _MIN_SIZE || n > _MAX_SIZE) {
    std::cerr << "Invalid input size (" << _MIN_SIZE << "-" << _MAX_SIZE << ")!"
              << std::endl;
    return;
  }

  auto [board, steps] = tiling::solve(n, true);
  steps.push_back(board);

  std::string path = visualize::visualizeSteps(steps, _VISUAL_DIR);

  std::cout << board.getSquares().size() << std::endl;
  for (const Square& square : board.getSquares()) {
    std::cout << square.x + 1 << " " << square.y + 1 << " " << square.size
              << std::endl;
  }

  std::cout << "Generated steps animation: " << path << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    return 1;
  }

  std::string mode = argv[1];
  const std::map<std::string, std::function<void()>> modes = {
      {"solve", solveMode},
      {"benchmark", benchmarkMode},
      {"visualize", visualizeMode},
      {"steps", stepsMode}};

  auto it = modes.find(mode);
  if (it != modes.end()) {
    it->second();
  }

  return 0;
}