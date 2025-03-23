#ifndef JSONHANDLER_HPP_
#define JSONHANDLER_HPP_

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

class JsonHandler {
 private:
  std::fstream file;

 public:
  JsonHandler(const std::string& filename);

  void write(nlohmann::json& json);
  nlohmann::json read();

  ~JsonHandler();
};

#endif  // JSONHANDLER_HPP_