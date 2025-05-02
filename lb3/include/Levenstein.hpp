#ifndef LEVENSTEIN_HPP_
#define LEVENSTEIN_HPP_

#include <string>
#include <utility>
#include <vector>

#include "Cost.hpp"

namespace levenstein {

using matrix_t = std::vector<std::vector<int>>;

matrix_t getMatrix(const std::string& s1, const std::string& s2,
                   const Cost& cost, bool record);

std::string getPrescription(const matrix_t& matrix, const std::string& s1,
                            const std::string& s2, const Cost& cost,
                            bool record);

void getAllPrescriprions(const matrix_t& matrix_t, const std::string& s1,
                         const std::string& s2, const Cost& cost,
                         std::vector<std::string>& prescriptions,
                         std::string& currentPresrciption,
                         std::pair<int, int> pos, bool record);

void show(const std::string& s1, const std::string& s2, const std::string& prescription);

}  // namespace levenstein

#endif  // LEVENSTEIN_HPP_