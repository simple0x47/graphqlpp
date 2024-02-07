// Copyright (c) Gabriel Amihalachioaie, SimpleG 2024.

#include "tokenizer.h"

constexpr char32_t SPECIAL_VALID_SOURCE_CHARACTERS[] = {
    U'\U00000009', U'\U0000000A', U'\U0000000D'};

namespace graphqlpp::language::tokenization {

Result<std::vector<Token>, TokenizeError> invalid_source_character_error(
    size_t line, size_t column);

/// \brief Checks whether a source character is a line terminator.
/// \param source GraphQL source text.
/// \param i Current character within the source text.
/// \return The first boolean indicates whether the character is a line
/// terminator, and the second boolean indicates if the line terminator is a
/// '\r\n'.
std::tuple<bool, bool> is_source_character_a_line_terminator(
    const std::vector<char32_t>& source, size_t i);

Result<std::vector<Token>, TokenizeError> tokenize(
    const std::vector<char32_t>& source) {
  std::vector<Token> tokens = std::vector<Token>();
  size_t line = 1;
  size_t column = 1;

  // TODO: #1 handle more than `long`'s maximum value characters within a
  // document.
  if (source.size() > std::numeric_limits<long>::max()) {
    return Result<std::vector<Token>, TokenizeError>::Err(
        TokenizeError("The document exceeds the maximum value of 'long'.",
                      std::vector{Location{.line_ = line, .column_ = column}},
                      std::nullopt, std::nullopt));
  }

  long token_start = 0;
  TokenType token_type;
  bool is_token_end = false;

  for (long i = 0; i < source.size(); i++) {
    char32_t s = source.at(i);

    if (!is_source_character_valid(s)) {
      return invalid_source_character_error(line, column);
    }

    auto [is_line_terminator, is_carriage_return_and_new_line] =
        is_source_character_a_line_terminator(source, i);

    if (is_line_terminator) {
      token_type = LINE_TERMINATOR;
      line++;
      column = 1;

      if (is_carriage_return_and_new_line) {
        i++;
      }

      is_token_end = true;
    }

    if (token_type != LINE_TERMINATOR) {
      column++;
    }

    if (is_token_end) {
      tokens.push_back(Token{.value_ = std::vector(source.begin() + token_start,
                                                   source.begin() + i + 1),
                             .type_ = token_type,
                             .ignored_ = is_token_type_ignored(token_type)});

      token_start = i + 1;
      is_token_end = false;
    }
  }

  return Result<std::vector<Token>, TokenizeError>::Ok(tokens);
}

bool is_source_character_valid(const char32_t source_character) {
  for (const char32_t s : SPECIAL_VALID_SOURCE_CHARACTERS) {
    if (source_character == s) {
      return true;
    }
  }

  if (source_character >= U'\U00000020' && source_character <= U'\U0000FFFF') {
    return true;
  }

  return false;
}

bool is_token_type_ignored(TokenType type) {
  switch (type) {
    case PUNCTUATOR:
    case NAME:
    case INT_VALUE:
    case FLOAT_VALUE:
    case STRING_VALUE:
      return false;
    case UNICODE_BOM:
    case WHITESPACE:
    case LINE_TERMINATOR:
    case COMMENT:
    case COMMA:
    default:
      return true;
  }
}

Result<std::vector<Token>, TokenizeError> invalid_source_character_error(
    size_t line, size_t column) {
  std::vector<Location> locations = std::vector<Location>();
  locations.push_back(Location{.line_ = line, .column_ = column});

  return Result<std::vector<Token>, TokenizeError>::Err(
      TokenizeError("Detected an invalid Unicode character.",
                    std::move(locations), std::nullopt, std::nullopt));
}

std::tuple<bool, bool> is_source_character_a_line_terminator(
    const std::vector<char32_t>& source, size_t i) {
  const char32_t s = source.at(i);

  bool is_line_terminator = false;
  bool is_carriage_return_and_new_line = false;

  if (s == NEW_LINE) {
    is_line_terminator = true;
  }

  if (s == CARRIAGE_RETURN) {
    const char32_t n = source.at(i + 1);

    is_line_terminator = true;

    if (n == NEW_LINE) {
      is_carriage_return_and_new_line = true;
    }
  }

  return std::make_tuple(is_line_terminator, is_carriage_return_and_new_line);
}
}  // namespace graphqlpp::language::tokenization