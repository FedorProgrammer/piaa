#include <string>
#include <vector>

#include "BenchmarkResult.hpp"

class Benchmark {
 private:
  int iterations;
  std::vector<BenchmarkResult> benchmarkResults;

 public:
  Benchmark(int iterations);

  void run(size_t minSize, size_t maxSize);
  void save(const std::string& ouput) const;

  const std::vector<BenchmarkResult>& getResults() const {
    return benchmarkResults;
  }
};