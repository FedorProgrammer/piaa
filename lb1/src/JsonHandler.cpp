#include "../include/JsonHandler.hpp"

JsonHandler::JsonHandler(const std::string& filename) {
  file.open(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Couldn't open a json-file!");
  }
}

void JsonHandler::write(nlohmann::json& json) { file << json.dump(2); }

nlohmann::json JsonHandler::read() {
  nlohmann::json json = nlohmann::json::object();
  file >> json;
  return json;
}

JsonHandler::~JsonHandler() {
  if (file.is_open()) {
    file.close();
  }
}