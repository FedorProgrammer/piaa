#include "Trie.hpp"

#include <iostream>
#include <map>

#include "Vertex.hpp"

Trie::Trie() : vertices(1, new Vertex(false, '\0')), patternsCount(0) {}

void Trie::push(const std::string& pattern, bool record) {
  if (record) {
    std::cout << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "[Trie::push] вход" << std::endl;
    std::cout << "pattern:  [" << pattern << "]" << std::endl;
  }

  int idx_V = 0;
  for (char symbol : pattern) {
    if (record) {
      std::cout << "--------------------------------" << std::endl;
      std::cout << "symbol: [" << symbol << "]" << std::endl;
      std::cout << "idx_V:  [" << idx_V << "]" << std::endl;
      std::cout << "vertices.size(): [" << vertices.size() << "]" << std::endl;
    }

    if (!vertices[idx_V]->next.count(symbol)) {
      if (record) {
        std::cout << "Нет прямого перехода из текущей вершины" << std::endl;
        std::cout << "Создаем новую вершину" << std::endl;
      }

      Vertex* u = new Vertex(false, symbol);
      u->parentIdx = idx_V;
      vertices.push_back(u);
      vertices[idx_V]->next[symbol] = vertices.size() - 1;
    }

    idx_V = vertices[idx_V]->next[symbol];
    if (record) {
      std::cout << "idx_V:         [" << idx_V << "]" << std::endl;
      std::cout << "vertices size: [" << vertices.size() << "]" << std::endl;
      std::cout << "--------------------------------" << std::endl;
    }
  }

  patternsCount++;
  vertices[idx_V]->isTerminal = true;
  vertices[idx_V]->patternIndices.push_back(patternsCount - 1);

  if (record) {
    std::cout << "[Trie::push] выход" << std::endl;
    std::cout << "pattern: [" << pattern << "] добавлен в бор" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;
  }
}

bool Trie::find(const std::string& pattern, bool record) {
  if (record) {
    std::cout << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Поиск шаблона в боре" << std::endl;
    std::cout << "pattern: [" << pattern << "]" << std::endl;
  }

  int idx_V = 0;
  for (char symbol : pattern) {
    if (record) {
      std::cout << "--------------------------------" << std::endl;
      std::cout << "symbol: [" << symbol << "]" << std::endl;
      std::cout << "idx_V:  [" << idx_V << "]" << std::endl;
      std::cout << "vertices.size(): [" << vertices.size() << "]" << std::endl;
    }

    if (!vertices[idx_V]->next.count(symbol)) {
      if (record) {
        std::cout << "Нет перехода по символу - шаблон не найден" << std::endl;
        std::cout << "================================" << std::endl;
        std::cout << std::endl;
      }
      return false;
    }

    idx_V = vertices[idx_V]->next[symbol];
  }

  bool found = vertices[idx_V]->isTerminal;
  if (record) {
    std::cout << (found ? "Шаблон найден: достигнута терминальная вершина"
                        : "Шаблон не найден: достигнута нетерминальная вершина")
              << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;
  }
  return found;
}

Trie::~Trie() {
  for (auto idx_V : vertices) delete idx_V;
}