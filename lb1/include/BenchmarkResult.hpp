#include "Board.hpp"

struct BenchmarkResult {
  double time;
  Board board;

  BenchmarkResult(double time, Board result) : time(time), board(result) {}
};