#include "AhoCorasick.hpp"

#include <iostream>

#include "Trie.hpp"
#include "Vertex.hpp"

namespace ahocorasick {

namespace {

int getAutoMove(Trie& trie, int idx_V, char symbol, bool record);

int getSuffixLink(Trie& trie, int idx_V, bool record) {
  Vertex& vertex_V = trie.getVertex(idx_V);
  if (record) {
    std::cout << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "[getSuffixLink] вход" << std::endl;
    std::cout << "idx_V:      [" << idx_V << "]" << std::endl;
    std::cout << "symbol:     [" << vertex_V.symbol << "]" << std::endl;
    std::cout << "parentIdx:  [" << vertex_V.parentIdx << "]" << std::endl;
    std::cout << "suffixLink: [" << vertex_V.suffixLink << "]" << std::endl;
    std::cout << "--------------------------------" << std::endl;
  }

  if (vertex_V.suffixLink == -1) {
    if (record) {
      std::cout << "Суффиксной ссылки для этой вершины нет" << std::endl;
    }

    if (idx_V == 0 || vertex_V.parentIdx == 0) {
      if (record) {
        std::cout << "Попали либо в корень, либо в потомка корня" << std::endl;
      }

      vertex_V.suffixLink = 0;
    } else {
      if (record) {
        std::cout << "Переходим по суффиксной ссылке родителя" << std::endl;
      }

      vertex_V.suffixLink =
          getAutoMove(trie, getSuffixLink(trie, vertex_V.parentIdx, record),
                      vertex_V.symbol, record);
    }
  }

  if (record) {
    std::cout << "[getSuffixLink] выход" << std::endl;
    std::cout << "suffixLink: [" << vertex_V.suffixLink << "]" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;
  }
  return vertex_V.suffixLink;
}

int getOutputLink(Trie& trie, int idx_V, bool record) {
  Vertex& vertex_V = trie.getVertex(idx_V);
  if (record) {
    std::cout << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "[getOutputLink] вход" << std::endl;
    std::cout << "idx_V:      [" << idx_V << "]" << std::endl;
    std::cout << "outputLink: [" << vertex_V.outputLink << "]" << std::endl;
    std::cout << "--------------------------------" << std::endl;
  }

  if (vertex_V.outputLink == -1) {
    if (record) {
      std::cout << "Конечная ссылка для этой вершины неопределена" << std::endl;
      std::cout << "--------------------------------" << std::endl;
    }

    int idx_U = getSuffixLink(trie, idx_V, record);
    if (record) {
      std::cout << "suffixLink для idx_V=[" << idx_V << "]" << " -> idx_U=["
                << idx_U << "]" << std::endl;
      std::cout << "--------------------------------" << std::endl;
    }

    if (idx_U == 0) {
      if (record) {
        std::cout << "Попали в корень" << std::endl;
      }

      vertex_V.outputLink = 0;
    } else {
      if (record) {
        std::cout << "Проверяем isTerminal у idx_U" << std::endl;
      }

      Vertex& vertex_U = trie.getVertex(idx_U);
      if (vertex_U.isTerminal) {
        if (record) {
          std::cout << "Найдена конеченая вершина -> outputLink=[" << idx_U
                    << "]" << std::endl;
        }

        vertex_V.outputLink = idx_U;
      } else {
        if (record) {
          std::cout << "Конечная вершина не найдена -> продолжаем поиск через "
                       "рекурсию"
                    << std::endl;
        }
        vertex_V.outputLink = getOutputLink(trie, idx_U, record);
      }
    }
  }

  if (record) {
    std::cout << "[getOutputLink] выход" << std::endl;
    std::cout << "outputLink: [" << vertex_V.outputLink << "]" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;
  }
  return vertex_V.outputLink;
}

int getAutoMove(Trie& trie, int idx_V, char symbol, bool record) {
  Vertex& vertex_V = trie.getVertex(idx_V);
  if (record) {
    std::cout << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "[getAutoMove] вход" << std::endl;
    std::cout << "idx_V:  [" << idx_V << "]" << std::endl;
    std::cout << "symbol: [" << symbol << "]" << std::endl;
    std::cout << "--------------------------------" << std::endl;
  }

  if (!vertex_V.autoMove.count(symbol)) {
    if (vertex_V.next.count(symbol)) {
      if (record) {
        std::cout << "Есть прямой переход -> next_state=["
                  << vertex_V.next[symbol] << "]" << std::endl;
      }

      vertex_V.autoMove[symbol] = vertex_V.next[symbol];
    } else {
      if (record) {
        std::cout << "Нет прямого перехода, идём по suffixLink" << std::endl;
      }

      vertex_V.autoMove[symbol] =
          (idx_V == 0) ? 0
                       : getAutoMove(trie, getSuffixLink(trie, idx_V, record),
                                     symbol, record);
    }
  }

  if (record) {
    std::cout << "[getAutoMove] выход" << std::endl;
    std::cout << "next state: [" << vertex_V.autoMove[symbol] << "]"
              << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;
  }
  return vertex_V.autoMove[symbol];
}

Trie initTrie(const std::vector<std::string>& patterns, bool record) {
  if (record) {
    std::cout << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "[initTrie] вход" << std::endl;
    std::cout << "patterns size: [" << patterns.size() << "]" << std::endl;
    std::cout << "--------------------------------" << std::endl;
  }

  Trie trie;
  for (size_t i = 0; i < patterns.size(); ++i) {
    if (record) {
      std::cout << "patterns[" << i << "]: [" << patterns[i] << "]"
                << std::endl;
      std::cout << "--------------------------------" << std::endl;
    }
    trie.push(patterns[i], record);
  }

  if (record) {
    std::cout << "[initTrie] выход" << std::endl;
    std::cout << "vertices size: [" << trie.getVerticesCount() << "]"
              << std::endl;
    std::cout << "patterns count: [" << trie.getPatternsCount() << "]"
              << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;
  }
  return trie;
}

std::pair<std::vector<std::string>, std::vector<int>> makePartition(
    const std::string& pattern, char wildcard, bool record) {
  if (record) {
    std::cout << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "[makePartition] вход" << std::endl;
    std::cout << "pattern:  [" << pattern << "]" << std::endl;
    std::cout << "wildcard: [" << wildcard << "]" << std::endl;
    std::cout << "--------------------------------" << std::endl;
  }

  std::vector<std::string> pieces;
  std::vector<int> positions;

  std::string current;
  for (size_t i = 0; i < pattern.length(); ++i) {
    if (pattern[i] == wildcard) {
      if (!current.empty()) {
        pieces.push_back(current);
        int pos = int(i - current.length());
        positions.push_back(pos);

        if (record) {
          std::cout << "Кусок: [" << current << "]" << std::endl;
          std::cout << "pos:   [" << pos << "]" << std::endl;
          std::cout << "--------------------------------" << std::endl;
        }

        current.clear();
      }
    } else {
      current.push_back(pattern[i]);
    }
  }
  if (!current.empty()) {
    pieces.push_back(current);
    int pos = int(pattern.size() - current.size());
    positions.push_back(pos);

    if (record) {
      std::cout << "Кусок: [" << current << "]" << std::endl;
      std::cout << "pos:   [" << pos << "]" << std::endl;
      std::cout << "--------------------------------" << std::endl;
    }
  }

  if (record) {
    std::cout << "[makePartition] выход" << std::endl;
    std::cout << "Общее число кусков без джокера: [" << pieces.size() << "]"
              << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;
  }

  return {pieces, positions};
}

}  // namespace

std::vector<std::pair<int, int>> search(
    const std::string& text, const std::vector<std::string>& patterns,
    bool record) {
  if (record) {
    std::cout << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "[search-default] вход" << std::endl;
    std::cout << "text:          [" << text << "]" << std::endl;
    std::cout << "patterns size: [" << patterns.size() << "]" << std::endl;
    std::cout << "--------------------------------" << std::endl;
  }

  Trie trie = initTrie(patterns, record);
  std::vector<std::pair<int, int>> result;
  int state = 0;

  for (size_t i = 0; i < text.length(); i++) {
    state = getAutoMove(trie, state, text[i], record);

    if (record) {
      std::cout << "Шаг i:  [" << i << "] " << std::endl;
      std::cout << "symbol: [" << text[i] << "]" << std::endl;
      std::cout << "state:  [" << state << "]" << std::endl;
      std::cout << "--------------------------------" << std::endl;
    }

    for (int u = state; u != 0; u = getOutputLink(trie, u, record)) {
      Vertex& vertex_U = trie.getVertex(u);
      if (vertex_U.isTerminal) {
        for (int patternIdx : vertex_U.patternIndices) {
          int symbolIdx = i - patterns[patternIdx].length() + 1;
          result.push_back({symbolIdx, patternIdx});

          if (record) {
            std::cout << "Шаблон совпал" << std::endl;
            std::cout << "symbolIdx:  [" << symbolIdx << "]" << std::endl;
            std::cout << "patternIdx: [" << patternIdx << "]" << std::endl;
          }
        }
      }
    }
  }

  if (record) {
    std::cout << "[search-default] выход" << std::endl;
    std::cout << "Общее число совпадений: [" << result.size() << "]"
              << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;
  }

  return result;
}

std::vector<int> search(const std::string& text, const std::string& pattern,
                        char wildcard, bool record) {
  if (record) {
    std::cout << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "[search(wildcard)] вход" << std::endl;
    std::cout << "text:     [" << text << "]" << std::endl;
    std::cout << "pattern:  [" << pattern << "]" << std::endl;
    std::cout << "wildcard: [" << wildcard << "]" << std::endl;
    std::cout << "--------------------------------" << std::endl;
  }

  auto [pieces, positions] = makePartition(pattern, wildcard, record);

  size_t occurrencesCount = text.size() - pattern.size() + 1;
  std::vector<int> count(occurrencesCount, 0);

  std::vector<std::pair<int, int>> matches = search(text, pieces, record);

  if (record) {
    std::cout << "--------------------------------" << std::endl;
    std::cout << "pieces.size(): [" << pieces.size() << "]" << std::endl;
    std::cout << "Число возможных стартов: [" << occurrencesCount << "]"
              << std::endl;
    std::cout << "Всего найдено вхождений кусочков: [" << matches.size() << "]"
              << std::endl;
    std::cout << "--------------------------------" << std::endl;
  }

  for (auto [pos, pieceIdx] : matches) {
    int start = pos - positions[pieceIdx];
    if (start >= 0 && (size_t)start < occurrencesCount) {
      count[start]++;

      if (record) {
        std::cout << "pieceIdx:     [" << pieceIdx << "]" << std::endl;
        std::cout << "pos:          [" << pos << "]" << std::endl;
        std::cout << "start:        [" << start << "]" << std::endl;
        std::cout << "count[start]: [" << count[start] << std::endl;
        std::cout << "--------------------------------" << std::endl;
      }
    } else if (record) {
      std::cout << "Игнорируем кусок" << std::endl;
      std::cout << "pieceIdx: [" << pieceIdx << "]" << std::endl;
      std::cout << "pos:      [" << pos << "] (выходит за границы)"
                << std::endl;
      std::cout << "--------------------------------" << std::endl;
    }
  }

  std::vector<int> result;
  for (size_t i = 0; i < occurrencesCount; ++i) {
    if (count[i] == (int)pieces.size()) {
      if (record) {
        std::cout << "Полное совпадение на старте: [" << i << "]" << std::endl;
        std::cout << "--------------------------------" << std::endl;
      }
      result.push_back(i);
    }
  }

  if (record) {
    std::cout << "[search(wildcard)] выход" << std::endl;
    std::cout << "Число полных совпадений: [" << result.size() << "]"
              << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;
  }

  return result;
}

void buildAutomaton(Trie& trie, bool record) {
  if (record) {
    std::cout << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "[buildAutomaton] вход" << std::endl;
    std::cout << "Число вершин: [" << trie.getVerticesCount() << "]"
              << std::endl;
    std::cout << "--------------------------------" << std::endl;
  }

  for (int v = 0; v < trie.getVerticesCount(); v++) {
    if (record) {
      std::cout << "Обработка вершины с индексом [" << v << "]" << std::endl;
      std::cout << "--------------------------------" << std::endl;
    }

    getSuffixLink(trie, v, record);

    getOutputLink(trie, v, record);

    if (record) {
      std::cout << "Вершина [" << v << "] обработана" << std::endl;
      std::cout << "--------------------------------" << std::endl;
    }
  }

  if (record) {
    std::cout << "[buildAutomaton] выход" << std::endl;
    std::cout << "Автомат Ахо-Корасик полностью построен" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;
  }
}

}  // namespace ahocorasick