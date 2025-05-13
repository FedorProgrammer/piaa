#ifndef TRIE_HPP_
#define TRIE_HPP_

#include <string>
#include <vector>

struct Vertex;

class Trie {
 private:
  std::vector<Vertex *> vertices;
  int patternsCount;

 public:
  Trie();

  void push(const std::string &pattern, bool record);
  bool find(const std::string &pattern, bool record);

  int getVerticesCount() { return vertices.size(); }
  int getPatternsCount() { return patternsCount; }
  Vertex &getVertex(int num) { return *vertices[num]; };

  ~Trie();
};

#endif  // TRIE_HPP_