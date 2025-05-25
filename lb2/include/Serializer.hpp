#ifndef SERIALIZER_HPP_
#define SERIALIZER_HPP_

#include <nlohmann/json.hpp>
#include <vector>

namespace serialize {

nlohmann::json to_json(const std::vector<std::vector<double>>& matrix);

}  // namespace serialize

#endif  // SERIALIZER_HPP_