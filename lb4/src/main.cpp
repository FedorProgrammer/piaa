#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../include/kmp.hpp"

void findMode() {
  std::string pattern, text;
  std::cin >> pattern >> text;

  std::vector<int> matches = kmp::find(text, pattern, false);
  for (size_t i = 0; i < matches.size(); i++) {
    std::cout << matches[i];
    if (i < matches.size() - 1) {
      std::cout << ',';
    }
  }

  std::cout << std::endl;
}

void cycleMode() {
  std::string a, b;
  std::cin >> a >> b;

  int pos = kmp::is_cycle_shift(a, b);

  std::cout << pos << std::endl;
}

void stepMode() {
  std::string pattern, text;
  std::cin >> pattern >> text;

  std::vector<int> matches = kmp::find(text, pattern, true);
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    return 1;
  }

  std::string mode = argv[1];
  std::map<std::string, std::function<void()>> modes = {{"find", findMode},
                                                        {"cycle", cycleMode},
                                                        {"step", stepMode}};

  auto it = modes.find(mode);
  if (it != modes.end()) {
    it->second();
  }

  return 0;
}