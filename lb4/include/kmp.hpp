#ifndef KMP_HPP_
#define KMP_HPP_

#include <string>
#include <utility>
#include <vector>

namespace kmp {

std::vector<int> prefix_function(const std::string& text, bool record);

std::vector<int> find(const std::string& text, const std::string& pattern,
                      bool record);

int is_cycle_shift(const std::string& a, const std::string& b);

}  // namespace kmp

#endif  // KMP_HPP_