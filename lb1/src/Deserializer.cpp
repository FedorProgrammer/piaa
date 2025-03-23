#include "../include/Deserializer.hpp"

#include <stdexcept>

#include "../include/Board.hpp"
#include "../include/Square.hpp"

namespace deserialize {

namespace detail {

void validate_field(const nlohmann::json& json, const std::string& field_name,
                    const std::string& object_type) {
  if (!json.contains(field_name)) {
    throw std::runtime_error("Invalid " + object_type + " JSON: missing '" +
                             field_name + "' field in '" + object_type + "'");
  }
}

}  // namespace detail

Square get_square(const nlohmann::json& square_json) {
  detail::validate_field(square_json, "coordinates", "square");
  detail::validate_field(square_json, "size", "square");

  int x = square_json["coordinates"][0];
  int y = square_json["coordinates"][1];
  int size = square_json["size"];

  return Square({x, y}, size);
}

Board get_board(const nlohmann::json& board_json) {
  detail::validate_field(board_json, "size", "board");
  detail::validate_field(board_json, "squares", "board");

  int size = board_json["size"];
  Board board(size);

  for (const auto& square_json : board_json["squares"]) {
    Square square = deserialize::get_square(square_json);
    board.addSquare(square);
  }

  return board;
}

}  // namespace deserialuze