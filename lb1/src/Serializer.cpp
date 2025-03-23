#include "../include/Serializer.hpp"

#include "../include/Board.hpp"
#include "../include/Square.hpp"

namespace serialize {

  nlohmann::json to_json(const Square& square) {
    nlohmann::json square_json = nlohmann::json::object();
    nlohmann::json coordinates = nlohmann::json::array();
  
    coordinates[0] = square.x;
    coordinates[1] = square.y;
  
    square_json["size"] = square.size;
    square_json["coordinates"] = coordinates;
  
    return square_json;
  }
  
  nlohmann::json to_json(const Board& board) {
    nlohmann::json board_json = nlohmann::json::object();
  
    nlohmann::json squares_json = nlohmann::json::array();
    for (const auto& square : board.getSquares()) {
      nlohmann::json square_json = to_json(square);
      squares_json.push_back(square_json);
    }
  
    board_json["size"] = board.getSize();
    board_json["squares"] = squares_json;
  
    return board_json;
  }
  
} // namespace serialize