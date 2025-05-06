#include "../include/Levenstein.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>

namespace levenstein {

namespace details {

const std::string red = "\033[31m";
const std::string reset = "\033[0m";

matrix_t initMatrix(int n, int m, const Cost& cost) {
  matrix_t matrix(n + 1, std::vector<int>(m + 1, 0));

  for (int i = 1; i < n + 1; i++) {
    matrix[i][0] = matrix[i - 1][0] + cost.deleteCost;
  }

  for (int j = 1; j < m + 1; j++) {
    matrix[0][j] = matrix[0][j - 1] + cost.insertCost;
  }

  return matrix;
}

void printMatrix(const matrix_t& matrix) {
  if (matrix.empty()) return;

  size_t rows = matrix.size();
  size_t cols = matrix[0].size();

  int maxVal = 0;
  for (auto& row : matrix) {
    for (int x : row) {
      maxVal = std::max(maxVal, x);
    }
  }

  size_t numberWidth = std::to_string(maxVal).size();
  size_t columnWidth = std::to_string(cols - 1).size();
  size_t rowWidth = std::to_string(rows - 1).size();

  size_t width = std::max({numberWidth, columnWidth, rowWidth}) + 1;

  std::cout << std::setw(width) << ' ';
  for (size_t j = 0; j < cols; j++) {
    std::cout << std::setw(width) << j;
  }
  std::cout << std::endl;

  for (size_t i = 0; i < rows; i++) {
    std::cout << std::setw(width) << i;
    for (size_t j = 0; j < cols; j++) {
      std::cout << std::setw(width) << matrix[i][j];
    }
    std::cout << std::endl;
  }
}

}  // namespace details

matrix_t getMatrix(const std::string& s1, const std::string& s2,
                   const Cost& cost, bool record) {
  int n = s1.length();
  int m = s2.length();

  if (record) {
    std::cout << std::endl;
    std::cout << "==========================" << std::endl;

    std::cout
        << "Алгоритм Фишера-Вагнера для нахождения расстояния Левенштейна:"
        << std::endl;
    std::cout << "\t1) Инициализация матрицы:" << std::endl;
    std::cout << "\t\t- Первая строка - стоимость превращений любой подстроки "
                 "s1 в пустую"
              << std::endl;
    std::cout << "\t\t- Первый столбец - стоимость превращений пустой строки "
                 "s1 в любую подстроку s2"
              << std::endl;

    std::cout << "\t2) Заполнение матрицы по правилу:" << std::endl;
    std::cout << "\t\t- s1[i] != s2[j]: matrix[i][j]= min(matrix[i - 1][j] + "
                 "deleteCost, matrix[i][j - 1] + insertCost, "
                 "matrix[i - 1][j - 1] + replaceCost)"
              << std::endl;
    std::cout << "\t\t- s1[i] == s2[j]: matrix[i][j] = matrix[i-1][j-1]"
              << std::endl;

    std::cout << "\t3) D(s1, s2) = matrix[n+1][m+1]" << std::endl;

    std::cout << "==========================" << std::endl;
    std::cout << std::endl;
  }

  matrix_t matrix = details::initMatrix(n, m, cost);

  for (int i = 1; i < n + 1; i++) {
    for (int j = 1; j < m + 1; j++) {
      if (s1[i - 1] != s2[j - 1]) {
        if (record) {
          std::cout << std::endl;
          std::cout << "--------------------------" << std::endl;

          std::cout << "i-1 = [" << i - 1 << "]" << std::endl;
          std::cout << "j-1 = [" << j - 1 << "]" << std::endl;
          std::cout << "s1[i-1] != s2[j-1]: " << std::endl;
          std::cout << "[s1]: [" << s1.substr(0, i - 1) << details::red
                    << s1[i - 1] << details::reset << s1.substr(i) << "]"
                    << std::endl;
          std::cout << "[s2]: [" << s2.substr(0, j - 1) << details::red
                    << s2[j - 1] << details::reset << s2.substr(j) << "]"
                    << std::endl;

          std::cout << "matrix[i][j] = min("
                    << matrix[i - 1][j] + cost.deleteCost << ", "
                    << matrix[i][j - 1] + cost.insertCost << ", "
                    << matrix[i - 1][j - 1] + cost.replaceCost << ")"
                    << std::endl;

          std::cout << "--------------------------" << std::endl;
          std::cout << std::endl;
        }

        matrix[i][j] = std::min({matrix[i - 1][j] + cost.deleteCost,
                                 matrix[i][j - 1] + cost.insertCost,
                                 matrix[i - 1][j - 1] + cost.replaceCost});
      } else {
        if (record) {
          std::cout << std::endl;
          std::cout << "--------------------------" << std::endl;

          std::cout << "i-1 = [" << i - 1 << "]" << std::endl;
          std::cout << "j-1 = [" << j - 1 << "]" << std::endl;
          std::cout << "s1[i-1] == s2[j-1]: " << std::endl;
          std::cout << "[s1]: [" << s1.substr(0, i - 1) << details::red
                    << s1[i - 1] << details::reset << s1.substr(i) << "]"
                    << std::endl;
          std::cout << "[s2]: [" << s2.substr(0, j - 1) << details::red
                    << s2[j - 1] << details::reset << s2.substr(j) << "]"
                    << std::endl;
          std::cout << "matrix[i][j] = " << matrix[i - 1][j - 1] << std::endl;

          std::cout << "--------------------------" << std::endl;
          std::cout << std::endl;
        }

        matrix[i][j] = matrix[i - 1][j - 1];
      }
    }
  }

  if (record) {
    std::cout << std::endl;
    std::cout << "==========================" << std::endl;

    std::cout << "Итоговая матрица: " << std::endl;
    details::printMatrix(matrix);

    std::cout << "==========================" << std::endl;
    std::cout << std::endl;
  }

  return matrix;
}

std::string getPrescription(const matrix_t& matrix, const std::string& s1,
                            const std::string& s2, const Cost& cost,
                            bool record) {
  int i = s1.length();
  int j = s2.length();
  std::string prescription;

  if (record) {
    std::cout << std::endl;
    std::cout << "==========================" << std::endl;

    std::cout << "Алгоритм для нахождения редакционного предписания:"
              << std::endl;
    std::cout << "\t1) Идем с конца двух строк" << std::endl;
    std::cout << "\t2) Работаем по матрице Фишера-Вагнера в обратном порядке"
              << std::endl;

    std::cout << "==========================" << std::endl;
    std::cout << std::endl;
  }

  while (i > 0 || j > 0) {
    if (i > 0 && j > 0 && s1[i - 1] == s2[j - 1]) {
      if (record) {
        std::cout << std::endl;
        std::cout << "--------------------------" << std::endl;

        std::cout << "Символы совпадают: " << s1[i - 1] << std::endl;
        std::cout << "i-1 = [" << i - 1 << "]" << std::endl;
        std::cout << "j-1 = [" << j - 1 << "]" << std::endl;
        std::cout << "Действие: M" << std::endl;

        std::cout << "--------------------------" << std::endl;
        std::cout << std::endl;
      }

      prescription.push_back('M');
      i--;
      j--;
    } else if (j > 0 &&
               (i == 0 || matrix[i][j - 1] + cost.insertCost == matrix[i][j])) {
      if (record) {
        std::cout << std::endl;
        std::cout << "--------------------------" << std::endl;

        std::cout << "Необходимо вставить символ: " << s2[j - 1] << std::endl;
        std::cout << "i-1 = [" << i - 1 << "]" << std::endl;
        std::cout << "j-1 = [" << j - 1 << "]" << std::endl;
        std::cout << "Действие: I" << std::endl;

        std::cout << "--------------------------" << std::endl;
        std::cout << std::endl;
      }

      prescription.push_back('I');
      j--;
    } else if (i > 0 &&
               (j == 0 || matrix[i - 1][j] + cost.deleteCost == matrix[i][j])) {
      if (record) {
        std::cout << std::endl;
        std::cout << "--------------------------" << std::endl;

        std::cout << "Необходимо удалить символ: " << s1[i - 1] << std::endl;
        std::cout << "i-1 = [" << i - 1 << "]" << std::endl;
        std::cout << "j-1 = [" << j - 1 << "]" << std::endl;
        std::cout << "Действие: D" << std::endl;

        std::cout << "--------------------------" << std::endl;
        std::cout << std::endl;
      }

      prescription.push_back('D');
      i--;
    } else {
      if (record) {
        std::cout << std::endl;
        std::cout << "--------------------------" << std::endl;

        std::cout << "Необходимо заменить символ " << s1[i - 1] << " на "
                  << s2[j - 1] << std::endl;
        std::cout << "i-1 = [" << i - 1 << "]" << std::endl;
        std::cout << "j-1 = [" << j - 1 << "]" << std::endl;
        std::cout << "Действие: R" << std::endl;

        std::cout << "--------------------------" << std::endl;
        std::cout << std::endl;
      }

      prescription.push_back('R');
      i--;
      j--;
    }
  }

  std::reverse(prescription.begin(), prescription.end());

  if (record) {
    std::cout << std::endl;
    std::cout << "==========================" << std::endl;

    std::cout << "Итоговое предписание: " << prescription << std::endl;

    std::cout << "==========================" << std::endl;
    std::cout << std::endl;
  }

  return prescription;
}

void getAllPrescriprions(const matrix_t& matrix, const std::string& s1,
                         const std::string& s2, const Cost& cost,
                         std::vector<std::string>& prescriptions,
                         std::string& currentPrescription,
                         std::pair<int, int> pos, bool record) {
  auto [i, j] = pos;

  if (i == 0 && j == 0) {
    std::string p = currentPrescription;
    std::reverse(p.begin(), p.end());

    if (record) {
      std::cout << std::endl;
      std::cout << "==========================" << std::endl;

      std::cout << "Найдено предписание: " << p << std::endl;

      std::cout << "==========================" << std::endl;
      std::cout << std::endl;
    }

    prescriptions.push_back(p);
    return;
  }

  if (i > 0 && j > 0 && s1[i - 1] == s2[j - 1]) {
    currentPrescription.push_back('M');

    if (record) {
      std::cout << std::endl;
      std::cout << "--------------------------" << std::endl;

      std::cout << "Символы совпадают: " << s1[i - 1] << std::endl;
      std::cout << "i-1 = [" << i - 1 << "]" << std::endl;
      std::cout << "j-1 = [" << j - 1 << "]" << std::endl;
      std::cout << "Действие: M" << std::endl;
      std::cout << "Текущее предписание: "
                << currentPrescription.substr(0,
                                              currentPrescription.length() - 1)
                << details::red << currentPrescription.back() << details::reset
                << std::endl;

      std::cout << "--------------------------" << std::endl;
      std::cout << std::endl;
    }

    getAllPrescriprions(matrix, s1, s2, cost, prescriptions,
                        currentPrescription, {i - 1, j - 1}, record);
    currentPrescription.pop_back();
  }

  if (i > 0 && (j == 0 || matrix[i][j] == matrix[i - 1][j] + cost.deleteCost)) {
    currentPrescription.push_back('D');

    if (record) {
      std::cout << std::endl;
      std::cout << "--------------------------" << std::endl;

      std::cout << "Необходимо удалить символ: " << s1[i - 1] << std::endl;
      std::cout << "i-1 = [" << i - 1 << "]" << std::endl;
      std::cout << "j-1 = [" << j - 1 << "]" << std::endl;
      std::cout << "Действие: D" << std::endl;
      std::cout << "Текущее предписание: "
                << currentPrescription.substr(0,
                                              currentPrescription.length() - 1)
                << details::red << currentPrescription.back() << details::reset
                << std::endl;

      std::cout << "--------------------------" << std::endl;
      std::cout << std::endl;
    }

    getAllPrescriprions(matrix, s1, s2, cost, prescriptions,
                        currentPrescription, {i - 1, j}, record);
    currentPrescription.pop_back();
  }

  if (j > 0 && (i == 0 || matrix[i][j] == matrix[i][j - 1] + cost.insertCost)) {
    currentPrescription.push_back('I');

    if (record) {
      std::cout << std::endl;
      std::cout << "--------------------------" << std::endl;

      std::cout << "Необходимо вставить символ: " << s2[j - 1] << std::endl;
      std::cout << "i-1 = [" << i - 1 << "]" << std::endl;
      std::cout << "j-1 = [" << j - 1 << "]" << std::endl;
      std::cout << "Действие: I" << std::endl;
      std::cout << "Текущее предписание: "
                << currentPrescription.substr(0,
                                              currentPrescription.length() - 1)
                << details::red << currentPrescription.back() << details::reset
                << std::endl;

      std::cout << "--------------------------" << std::endl;
      std::cout << std::endl;
    }

    getAllPrescriprions(matrix, s1, s2, cost, prescriptions,
                        currentPrescription, {i, j - 1}, record);
    currentPrescription.pop_back();
  }

  if (i > 0 && j > 0 &&
      (matrix[i][j] == matrix[i - 1][j - 1] + cost.replaceCost)) {
    currentPrescription.push_back('R');

    if (record) {
      std::cout << std::endl;
      std::cout << "--------------------------" << std::endl;

      std::cout << "Необходимо заменить символ " << s1[i - 1] << " на "
                << s2[j - 1] << std::endl;
      std::cout << "i-1 = [" << i - 1 << "]" << std::endl;
      std::cout << "j-1 = [" << j - 1 << "]" << std::endl;
      std::cout << "Действие: R" << std::endl;
      std::cout << "Текущее предписание: "
                << currentPrescription.substr(0,
                                              currentPrescription.length() - 1)
                << details::red << currentPrescription.back() << details::reset
                << std::endl;

      std::cout << "--------------------------" << std::endl;
      std::cout << std::endl;
    }

    getAllPrescriprions(matrix, s1, s2, cost, prescriptions,
                        currentPrescription, {i - 1, j - 1}, record);
    currentPrescription.pop_back();
  }
}

void show(const std::string& s1, const std::string& s2,
          const std::string& prescription) {
  std::string current = s1;
  int s1Pos = 0;
  int s2Pos = 0;

  std::cout << "Исходная строка: " << current << std::endl;

  for (const char& operation : prescription) {
    switch (operation) {
      case 'M':
        std::cout << "M: Совпадение " << current[s1Pos++] << " --> "
                  << s2[s2Pos++] << std::endl;
        break;

      case 'D':
        std::cout << "D: Удаляем " << current[s1Pos] << std::endl;
        current.erase(s1Pos, 1);
        break;

      case 'I':
        std::cout << "I: Вставляем " << s2[s2Pos] << std::endl;
        current.insert(s1Pos++, 1, s2[s2Pos++]);
        break;

      case 'R':
        std::cout << "R: Заменяем " << current[s1Pos] << " --> " << s2[s2Pos]
                  << std::endl;
        current[s1Pos++] = s2[s2Pos++];
        break;

      default:
        std::cout << "Неизвестная операция: " << operation << std::endl;
        break;
    }

    std::cout << "Текущая строка: " << current << std::endl;
  }

  std::cout << "Результат: " << current << std::endl;
}

}  // namespace levenstein