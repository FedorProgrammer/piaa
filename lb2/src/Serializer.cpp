#include "../include/Serializer.hpp"

namespace serialize {

nlohmann::json to_json(const std::vector<std::vector<double>>& matrix) {
  nlohmann::json matrix_json = nlohmann::json::object();
  nlohmann::json weights = nlohmann::json::array();

  int n = matrix.size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      weights[i][j] = matrix[i][j];
    }
  }

  matrix_json["size"] = {n, n};
  matrix_json["weights"] = weights;

  return matrix_json;
}

}  // namespace serialize