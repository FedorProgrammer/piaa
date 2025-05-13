#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "AhoCorasick.hpp"
#include "Trie.hpp"
#include "Visualize.hpp"

#define VISUAL_DIR "./pic"

void defaultSearch() {
  std::string text;

  int n;
  std::cin >> text >> n;

  std::vector<std::string> patterns(n);
  for (int i = 0; i < n; i++) {
    std::cin >> patterns[i];
  }

  std::vector<std::pair<int, int>> result =
      ahocorasick::search(text, patterns, false);

  std::sort(result.begin(), result.end());
  for (const auto& match : result) {
    std::cout << match.first + 1 << ' ' << match.second + 1 << std::endl;
  }
}

void wildcardSearch() {
  std::string text;
  std::string pattern;
  char wildcard;

  std::cin >> text >> pattern >> wildcard;

  std::vector<int> result = ahocorasick::search(text, pattern, wildcard, false);

  std::sort(result.begin(), result.end());
  for (const auto& match : result) {
    std::cout << match + 1 << std::endl;
  }
}

void defaultStep() {
  std::string text;

  int n;
  std::cin >> text >> n;

  std::vector<std::string> patterns(n);
  for (int i = 0; i < n; i++) {
    std::cin >> patterns[i];
  }

  std::vector<std::pair<int, int>> result =
      ahocorasick::search(text, patterns, true);
  std::sort(result.begin(), result.end());
  for (const auto& match : result) {
    std::cout << match.first + 1 << ' ' << match.second + 1 << std::endl;
  }
}

void wildcardStep() {
  std::string text;
  std::string pattern;
  char wildcard;

  std::cin >> text >> pattern >> wildcard;

  std::vector<int> result = ahocorasick::search(text, pattern, wildcard, true);

  std::sort(result.begin(), result.end());
  for (const auto& match : result) {
    std::cout << match + 1 << std::endl;
  }
}

void visualizeAutomaton() {
  int n;
  std::cin >> n;

  std::vector<std::string> patterns(n);
  for (int i = 0; i < n; i++) {
    std::cin >> patterns[i];
  }

  Trie trie;
  for (const auto& pattern : patterns) {
    trie.push(pattern, false);
  }

  ahocorasick::buildAutomaton(trie, false);
  visualize::automatonToDot(trie, std::string(VISUAL_DIR) + "/automaton");
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    return 1;
  }

  std::map<std::string, std::function<void()>> modes = {
      {"run-default", defaultSearch},
      {"run-wildcard", wildcardSearch},
      {"step-default", defaultStep},
      {"step-wildcard", wildcardStep},
      {"visualize", visualizeAutomaton}};

  std::string mode = argv[1];

  auto it = modes.find(mode);
  if (it != modes.end()) {
    it->second();
  }

  return 0;
}