#include "../include/kmp.hpp"

#include <iostream>

namespace kmp {

namespace details {

const std::string red = "\033[31m";
const std::string reset = "\033[0m";

}  // namespace details

std::vector<int> prefix_function(const std::string& text, bool record) {
  int n = text.length();
  std::vector<int> prefix(n, 0);

  if (record) {
    std::cout << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "Свойства префикс-функции: " << std::endl;
    std::cout << "Пусть P(text, i) = k, где: P() - префикс функция, text - "
                 "обрабатываемый текст, i - максимальный индекс в text (срез)"
              << std::endl;

    std::cout << "Тогда: " << std::endl;
    std::cout
        << "\t1) Если text[i + 1] == text[k + 1], то: P(text, i + 1) = k + 1"
        << std::endl;
    std::cout << "\t2) text[1..P(text, k)] - префикс-суффикс строки text[1..i]"
              << std::endl;

    std::cout << std::endl;

    std::cout << "В данном алгоритме введены переменные: " << std::endl;
    std::cout << "\t1) j - конец максимального префикса для данной подстроки, "
                 "что совпадает с суффиксом"
              << std::endl;
    std::cout << "\t2) i - конец максимального суффикса для данной подстроки, "
                 "что совпадает с префиксом"
              << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << std::endl;
  }

  for (int i = 1; i < n; i++) {
    int j = prefix[i - 1];
    if (record) {
      std::cout << std::endl;
      std::cout << "--------------------------" << std::endl;
      std::cout << "Поиск максимального префикс-суффикса для подстроки: "
                << "[" << text.substr(0, i + 1) << "]" << std::endl;

      std::cout << "Значение [j]: " << "[" << j << "]" << std::endl;
      std::cout << "Значение [i]: " << "[" << i << "]" << std::endl;
    }

    while ((j > 0) && (text[i] != text[j])) {
      j = prefix[j - 1];
      if (record) {
        std::cout << "Пользуемся свойством (2): " << std::endl;
        std::cout << "Новое значение [j]: " << "[" << j << "]" << std::endl;
      }
    }

    if (text[i] == text[j]) {
      j++;
      if (record) {
        std::cout << "Пользуемся свойством (1): " << std::endl;
        std::cout << "Новое значение [j]: " << "[" << j << "]" << std::endl;
      }
    }

    prefix[i] = j;
    if (record) {
      std::cout << "Максимальный префикс-суффикс (длина): " << j << std::endl;
      std::cout << "Максимальный префикс-суффикс (значение): "
                << "[" << text.substr(0, j) << "]" << std::endl;
      std::cout << "--------------------------" << std::endl;
      std::cout << std::endl;
    }
  }

  if (record) {
    std::cout << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "Итоговый вектор префикс функции: " << std::endl;
    std::cout << "prefix = [";
    for (size_t i = 0; i < prefix.size(); i++) {
      std::cout << prefix[i];
      if (i < prefix.size() - 1) {
        std::cout << ", ";
      }
    }
    std::cout << "]" << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << std::endl;
  }

  return prefix;
}

std::vector<int> find(const std::string& text, const std::string& pattern,
                      bool record) {
  std::vector<int> matches;

  int n = text.length();
  int m = pattern.length();
  if (m > n) {
    return {-1};
  }

  if (record) {
    std::cout << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "В данном алгоритме КМП введены переменные: " << std::endl;
    std::cout << "\t1) j - индекс текущего элемента в шаблоне (pattern)"
              << std::endl;
    std::cout << "\t2) i - индекс текущего элемента в тексте (text)"
              << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << std::endl;
  }

  std::vector<int> prefix = prefix_function(pattern, record);

  int j = 0;
  for (int i = 0; i < n; i++) {
    if (record) {
      std::cout << std::endl;
      std::cout << "--------------------------" << std::endl;
      std::cout << "Значение [j]: " << "[" << j << "]" << std::endl;
      std::cout << "Значение [i]: " << "[" << i << "]" << std::endl;

      std::cout << "Текст [text]: " << "[" << text.substr(0, i) << details::red
                << text[i] << details::reset << text.substr(i + 1) << "]"
                << std::endl;

      std::cout << "Шаблон [pattern]: " << "[";
      std::cout << pattern.substr(0, j) << details::red << pattern[j]
                << details::reset << pattern.substr(j + 1);
      std::cout << "]" << std::endl;
    }

    while ((j > 0) && (text[i] != pattern[j])) {
      j = prefix[j - 1];
      if (record) {
        std::cout << std::endl;
        std::cout << "Символы text[i] и pattern[j] не совпали: " << std::endl;
        std::cout << "text[i] = " << "[" << text[i] << "]" << std::endl;
        std::cout << "pattern[j] = " << "[" << pattern[j] << "]" << std::endl;
        std::cout << "Новое значение [j]: " << "[" << j << "]" << std::endl;
      }
    }

    if (text[i] == pattern[j]) {
      j++;
      if (record) {
        std::cout << std::endl;
        std::cout << "Найдено совпадение символов: " << std::endl;
        std::cout << "Новое значение [j]: " << "[" << j << "]" << std::endl;
      }
    }

    if (j >= m) {
      matches.push_back(i - j + 1);
      if (record) {
        std::cout << std::endl;
        std::cout << "Найдена подстрока: " << "[" << text.substr(i - j + 1, j)
                  << "]" << std::endl;
        std::cout << "Начало подстроки в тексте [i - j + 1]: " << "["
                  << i - j + 1 << "]" << std::endl;
        std::cout << "Конец подстроки в тексте [i]: " << "[" << i << "]"
                  << std::endl;
      }
      j = prefix[j - 1];
    }

    if (record) {
      std::cout << "--------------------------" << std::endl;
      std::cout << std::endl;
    }
  }

  if (matches.size() == 0) {
    matches.push_back(-1);
  }

  if (record) {
    std::cout << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "Итоговый вектор совпадений: " << std::endl;
    std::cout << "matches = [";
    for (size_t i = 0; i < matches.size(); i++) {
      std::cout << matches[i];
      if (i < matches.size() - 1) {
        std::cout << ", ";
      }
    }
    std::cout << "]" << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << std::endl;
  }

  return matches;
}

int is_cycle_shift(const std::string& text, const std::string& shift) {
  if (text.length() != shift.length()) {
    return -1;
  }

  std::vector<int> matched = find(shift + shift, text, false);

  return matched[0];
}

}  // namespace kmp