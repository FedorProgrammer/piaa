#include "../include/Deserializer.hpp"

namespace deserialize {

std::vector<std::vector<double>> from_json(const nlohmann::json& matrix_json) {
  std::vector<std::vector<double>> matrix =
      matrix_json["weights"].get<std::vector<std::vector<double>>>();
  return matrix;
}

}  // namespace deserialize