#ifndef SERIALIZER_HPP_
#define SERIALIZER_HPP_

#include <nlohmann/json.hpp>

struct Square;
class Board;

namespace serialize {

nlohmann::json to_json(const Square& square);
nlohmann::json to_json(const Board& board);

}  // namespace serialize

#endif  // SERIALIZER_HPP_