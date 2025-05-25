#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "Deserializer.hpp"
#include "JsonHandler.hpp"
#include "Serializer.hpp"
#include "tsp.hpp"

#define JSON_FILE_ "json/matrix.json"

std::vector<std::vector<double>> generateRandomMatrix(int n) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 100);

  std::vector<std::vector<double>> matrix(n, std::vector<double>(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i][j] = i == j ? -1 : dis(gen);
    }
  }
  return matrix;
}

std::vector<std::vector<double>> generateSymmetricMatrix(int n) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 100);

  std::vector<std::vector<double>> matrix(n, std::vector<double>(n));
  for (int i = 0; i < n; i++) {
    for (int j = i; j < n; j++) {
      int value = (i == j) ? -1 : dis(gen);
      matrix[i][j] = value;
      matrix[j][i] = value;
    }
  }
  return matrix;
}

std::vector<std::vector<double>> inputMatrix(int n) {
  std::vector<std::vector<double>> matrix(n, std::vector<double>(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cin >> matrix[i][j];
    }
  }
  return matrix;
}

void saveMatrixToFile(const std::vector<std::vector<double>>& matrix,
                      const std::string& filename) {
  try {
    JsonHandler handler(filename);
    nlohmann::json json = serialize::to_json(matrix);
    handler.write(json);
    std::cout << "Matrix saved in: " << filename << std::endl;

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

std::vector<std::vector<double>> loadMatrixFromFile(
    const std::string& filename) {
  try {
    JsonHandler handler(filename);
    nlohmann::json json = handler.read();
    return deserialize::from_json(json);

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    throw;
  }
}

void printMatrix(const std::vector<std::vector<double>>& matrix) {
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

void runAlgorithm(const std::vector<std::vector<double>>& weights,
                  bool stepMode) {
  auto [bnbPath, bnbWeight] = tsp::bnbSearch(weights, stepMode);

  std::cout << std::endl;
  std::cout << "================================" << std::endl;
  std::cout << "Branch and boundary solutuion: [";

  for (size_t i = 0; i < bnbPath.size(); i++) {
    if (i < bnbPath.size() - 1) {
      std::cout << bnbPath[i] << ' ';
    } else {
      std::cout << bnbPath[i] << ']' << std::endl;
    }
  }

  std::cout << std::fixed << std::setprecision(1) << "bnb weight: [" << bnbWeight
            << ']' << std::endl;

  auto [amrPath, amrWeight] = tsp::amrSearch(weights, stepMode);

  std::cout << "================================" << std::endl;
  std::cout << "AMR solution: [";

  for (size_t i = 0; i < amrPath.size(); i++) {
    if (i < amrPath.size() - 1) {
      std::cout << amrPath[i] << ' ';
    } else {
      std::cout << amrPath[i] << ']' << std::endl;
    }
  }

  std::cout << std::fixed << std::setprecision(1) << "amr weight: [" << amrWeight
            << ']' << std::endl;
  std::cout << "accuracy: [" << amrWeight / bnbWeight << ']' << std::endl;
}

int main() {
  std::cout << std::endl;
  std::cout << "Choose matrix generate mode:\n";
  std::cout << "1 - manual" << std::endl;
  std::cout << "2 - random" << std::endl;
  std::cout << "3 - random symmetric" << std::endl;
  std::cout << "4 - load from file" << std::endl;

  int choice;
  std::cin >> choice;

  std::vector<std::vector<double>> matrix;

  try {
    if (choice == 4) {
      matrix = loadMatrixFromFile(JSON_FILE_);
      printMatrix(matrix);
    } else {
      int n;
      std::cout << "Enter matrix size (n): ";
      std::cin >> n;

      if (n <= 0) {
        std::cerr << "Incorrect matrix size!" << std::endl;
        return 1;
      }

      switch (choice) {
        case 1:
          matrix = inputMatrix(n);
          break;
        case 2:
          matrix = generateRandomMatrix(n);
          saveMatrixToFile(matrix, JSON_FILE_);
          printMatrix(matrix);
          break;
        case 3:
          matrix = generateSymmetricMatrix(n);
          saveMatrixToFile(matrix, JSON_FILE_);
          printMatrix(matrix);
          break;
        default:
          std::cerr << "Unknown choice!\n";
          return 1;
      }
    }

    std::cout << std::endl;
    std::cout << "Choose mode:" << std::endl;
    std::cout << "1 - default" << std::endl;
    std::cout << "2 - step" << std::endl;

    int mode;
    std::cin >> mode;

    switch (mode) {
      case 1:
        runAlgorithm(matrix, false);
        break;
      case 2:
        runAlgorithm(matrix, true);
        break;
      default:
        std::cerr << "Unknown choice!\n";
        return 1;
    }

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}