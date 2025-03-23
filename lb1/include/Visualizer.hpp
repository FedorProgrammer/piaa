#ifndef VISUALIZER_HPP_
#define VISUALIZER_HPP_

#include <string>
#include <vector>

class Board;

namespace visualize {

std::string visualizeTiling(const Board& board, const std::string& title,
                            const std::string& filename,
                            const std::string& output_dir);
std::string visualizeExecutionTimes(const std::vector<int>& sizes,
                                    const std::vector<double>& times,
                                    const std::string& filename,
                                    const std::string& output_dir);
std::string visualizeSteps(const std::vector<Board>& steps,
                           const std::string& output_dir);

}  // namespace visualize

#endif  // VISUALIZER_HPP_