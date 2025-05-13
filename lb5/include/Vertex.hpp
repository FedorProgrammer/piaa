#ifndef VERTEX_HPP_
#define VERTEX_HPP_

#include <map>
#include <vector>

struct Vertex {
  int parentIdx;
  char symbol;

  int suffixLink;
  int outputLink;

  bool isTerminal;
  std::vector<int> patternIndices;

  std::map<char, int> next;
  std::map<char, int> autoMove;

  Vertex(bool isTerminal, char symbol)
      : parentIdx(-1),
        symbol(symbol),
        suffixLink(-1),
        outputLink(-1),
        isTerminal(isTerminal),
        patternIndices() {}
};

#endif  // VERTEX_HPP_