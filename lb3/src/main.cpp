#include <functional>
#include <iostream>
#include <map>
#include <string>

#include "../include/Cost.hpp"
#include "../include/Levenstein.hpp"

void runMode(const Cost& cost, const std::string& s1, const std::string& s2) {
  levenstein::matrix_t matrix = levenstein::getMatrix(s1, s2, cost, false);

  std::cout << std::endl;
  std::cout << "--------------------------" << std::endl;
  std::cout << matrix.back().back() << std::endl;
  std::cout << "--------------------------" << std::endl;
  std::cout << std::endl;

  std::string prescription =
      levenstein::getPrescription(matrix, s1, s2, cost, false);

  std::cout << std::endl;
  std::cout << "--------------------------" << std::endl;
  std::cout << prescription << std::endl;
  std::cout << s1 << std::endl;
  std::cout << s2 << std::endl;
  std::cout << "--------------------------" << std::endl;
  std::cout << std::endl;
}

void stepMode(const Cost& cost, const std::string& s1, const std::string& s2) {
  levenstein::matrix_t matrix = levenstein::getMatrix(s1, s2, cost, true);

  std::cout << std::endl;
  std::cout << "--------------------------" << std::endl;
  std::cout << "Редакционное расстояние: [" << matrix.back().back() << "]"
            << std::endl;
  std::cout << "--------------------------" << std::endl;
  std::cout << std::endl;

  std::string prescription =
      levenstein::getPrescription(matrix, s1, s2, cost, true);

  std::cout << std::endl;
  std::cout << "--------------------------" << std::endl;
  std::cout << "Редакционное предписание: [" << prescription << "]"
            << std::endl;
  levenstein::show(s1, s2, prescription);
  std::cout << "--------------------------" << std::endl;
  std::cout << std::endl;
}

void prescriptionsRunMode(const Cost& cost, const std::string& s1,
                          const std::string& s2) {
  levenstein::matrix_t matrix = levenstein::getMatrix(s1, s2, cost, false);

  std::vector<std::string> prescriptions;
  std::string current = "";
  levenstein::getAllPrescriprions(matrix, s1, s2, cost, prescriptions, current,
                                  {s1.length(), s2.length()}, false);

  for (size_t i = 0; i < prescriptions.size(); i++) {
    std::cout << std::endl;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Предписание номер [" << i << "]: " << prescriptions[i]
              << std::endl;
    std::cout << "--------------------------" << std::endl;
  }
}

void prescriptionsStepMode(const Cost& cost, const std::string& s1,
                           const std::string& s2) {
  levenstein::matrix_t matrix = levenstein::getMatrix(s1, s2, cost, true);

  std::vector<std::string> prescriptions;
  std::string current = "";
  levenstein::getAllPrescriprions(matrix, s1, s2, cost, prescriptions, current,
                                  {s1.length(), s2.length()}, true);

  for (size_t i = 0; i < prescriptions.size(); i++) {
    std::cout << std::endl;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Предписание номер [" << i << "]:" << std::endl;
    levenstein::show(s1, s2, prescriptions[i]);
    std::cout << "--------------------------" << std::endl;
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    return 1;
  }

  std::string mode = argv[1];

  Cost cost;
  std::string s1, s2;

  std::cin >> cost.replaceCost >> cost.insertCost >> cost.deleteCost;
  std::cin >> s1 >> s2;

  std::map<std::string, std::function<void(const Cost&, const std::string&,
                                           const std::string&)>>
      modes = {{"run", runMode},
               {"step", stepMode},
               {"prescriptions-no-step", prescriptionsRunMode},
               {"prescriptions-with-step", prescriptionsStepMode}};

  auto it = modes.find(mode);
  if (it != modes.end()) {
    it->second(cost, s1, s2);
  }

  return 0;
}