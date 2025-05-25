#ifndef DESERIALIZER_HPP_
#define DESERIALIZER_HPP_

#include <nlohmann/json.hpp>
#include <vector>

namespace deserialize {

std::vector<std::vector<double>> from_json(const nlohmann::json& matrix_json);

}  // namespace deserialize

#endif  // DESERIALIZER_HPP_