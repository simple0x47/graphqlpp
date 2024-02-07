// Copyright (c) Gabriel Amihalachioaie, SimpleG 2024.

#ifndef TOKENIZE_ERROR_H
#define TOKENIZE_ERROR_H
#include <utility>
#include <vector>

#include "extension.h"
#include "location.h"

namespace graphqlpp::language::tokenization {
class TokenizeError {
 private:
  std::string message_;
  std::optional<std::vector<Location>> locations_;
  std::optional<std::vector<std::string>> path_;
  std::optional<std::vector<Extension>> extensions_;

 public:
  TokenizeError(std::string message,
                std::optional<std::vector<Location>> locations,
                std::optional<std::vector<std::string>> path,
                std::optional<std::vector<Extension>> extensions) {
    message_ = std::move(message);
    locations_ = std::move(locations);
    path_ = std::move(path);
    extensions_ = std::move(extensions);
  }

  std::optional<std::vector<Location>> get_locations() { return locations_; }
};
}  // namespace graphqlpp::language::tokenization

#endif  // TOKENIZE_ERROR_H
