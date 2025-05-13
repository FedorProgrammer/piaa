#ifndef VISUALIZE_HPP_
#define VISUALIZE_HPP_

#include "Trie.hpp"

namespace visualize {
void automatonToDot(Trie& trie, const std::string& filename);
}  // namespace visualize

#endif  // VISUALIZE_HPP_