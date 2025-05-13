#ifndef AHOCORASICK_HPP_
#define AHOCORASICK_HPP_

#include <string>
#include <utility>
#include <vector>

class Trie;

namespace ahocorasick {

std::vector<std::pair<int, int>> search(
    const std::string& text, const std::vector<std::string>& patterns,
    bool record);

std::vector<int> search(const std::string& text, const std::string& pattern,
                        char wildcard, bool record);

void buildAutomaton(Trie& trie, bool record);

}  // namespace ahocorasick

#endif  // AHOCORASICK_HPP_