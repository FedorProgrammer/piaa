#ifndef DESERIALIZER_HPP_
#define DESERIALIZER_HPP_

#include <nlohmann/json.hpp>

struct Square;
class Board;

namespace deserialize {
                      
Square get_square(const nlohmann::json& square_json);
Board get_board(const nlohmann::json& board_json);

}  // namespace deserialize

#endif  // DESERIALIZER_HPP_