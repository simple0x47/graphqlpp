// Copyright (c) Gabriel Amihalachioaie, SimpleG 2024.

#ifndef TOKEN_H
#define TOKEN_H

#include <vector>

namespace graphqlpp::language::tokenization {
enum TokenType {
  // Lexical
  PUNCTUATOR,
  NAME,
  INT_VALUE,
  FLOAT_VALUE,
  STRING_VALUE,
  // Ignored
  UNICODE_BOM,
  WHITESPACE,
  LINE_TERMINATOR,
  COMMENT,
  COMMA
};

/// \brief GraphQL token.
struct Token {
  std::vector<char32_t> value_;
  TokenType type_;
  bool ignored_;
  /// \brief Line where the token starts.
  size_t line_;
  /// \brief Column where the token starts.
  size_t column_;

  bool operator==(const Token& other) const {
    return ignored_ == other.ignored_ && type_ == other.type_ &&
           line_ == other.line_ && column_ == other.column_ &&
           std::ranges::equal(value_, other.value_);
  }
};
}  // namespace graphqlpp::language::tokenization

#endif  // TOKEN_H