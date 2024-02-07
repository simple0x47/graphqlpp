// Copyright (c) Gabriel Amihalachioaie, SimpleG 2024.

#include <graphqlpp/language/tokenization/tokenizer.h>
#include <gtest/gtest.h>

#include "graphqlpp/language/tokenization/tokenize_error.h"
#include "graphqlpp/result.h"

using namespace graphqlpp;
using namespace graphqlpp::language::tokenization;

class DetectIllegalCharactersTestFixture
    : public testing::TestWithParam<char32_t> {};

TEST_P(DetectIllegalCharactersTestFixture,
       IsSourceCharacterValid_DetectIllegalCharacters) {
  const char32_t source_character = GetParam();

  ASSERT_FALSE(is_source_character_valid(source_character));
}

INSTANTIATE_TEST_SUITE_P(IllegalCharactersTest,
                         DetectIllegalCharactersTestFixture,
                         testing::Values(U'\U0001FFFF', U'\U0002FFFF'));

class TokenizeDetectIllegalCharactersTestFixture
    : public testing::TestWithParam<
          std::tuple<std::vector<char32_t>, size_t, size_t>> {};

TEST_P(TokenizeDetectIllegalCharactersTestFixture,
       Tokenize_DetectIllegalCharacters) {
  constexpr size_t expected_locations = 1;
  const auto [illegal_source, line, column] = GetParam();

  Result<std::vector<Token>, TokenizeError> r = tokenize(illegal_source);

  ASSERT_FALSE(r.IsOk());

  std::unique_ptr<TokenizeError> e = r.UnwrapErr();
  std::optional<std::vector<Location>> o = e->get_locations();

  ASSERT_TRUE(o.has_value());

  std::vector<Location> l = o.value();

  ASSERT_EQ(expected_locations, l.size());

  Location location = l.at(0);

  ASSERT_EQ(line, location.line_);
  ASSERT_EQ(column, location.column_);
}

INSTANTIATE_TEST_SUITE_P(
    IllegalCharactersTest, TokenizeDetectIllegalCharactersTestFixture,
    testing::Values(
        std::make_tuple(std::vector<char32_t>{U'\U0000FFFF', U'\U0001FFFF'}, 1,
                        2),
        std::make_tuple(std::vector<char32_t>{U'\U0000ABCD', U'\U0000AC21',
                                              NEW_LINE, U'\U0005FFFF'},
                        2, 1),
        std::make_tuple(std::vector<char32_t>{U'\U0000ABCD', U'\U0000AC21',
                                              CARRIAGE_RETURN, U'\U0005FFFF'},
                        2, 1),
        std::make_tuple(std::vector<char32_t>{U'\U0000ABCD', U'\U0000AC21',
                                              CARRIAGE_RETURN, NEW_LINE,
                                              U'\U0005FFFF'},
                        2, 1)));

class TokenizeDetectLineTerminatorsTestFixture
    : public testing::TestWithParam<
          std::tuple<std::vector<char32_t>, std::vector<Token>>> {};

TEST_P(TokenizeDetectLineTerminatorsTestFixture,
       Tokenize_DetectLineTerminators) {
  const auto [source, expected_tokens] = GetParam();

  Result<std::vector<Token>, TokenizeError> r = tokenize(source);

  ASSERT_TRUE(r.IsOk());

  std::unique_ptr<std::vector<Token>> tokens = r.Unwrap();

  ASSERT_EQ(expected_tokens.size(), tokens->size());

  for (size_t i = 0; i < expected_tokens.size(); i++) {
    Token expected_token = expected_tokens.at(i);
    Token token = tokens->at(i);

    ASSERT_EQ(expected_token, token);
  }
}

INSTANTIATE_TEST_SUITE_P(
    LineTerminatorsTest, TokenizeDetectLineTerminatorsTestFixture,
    testing::Values(std::make_tuple(
        std::vector<char32_t>{CARRIAGE_RETURN, NEW_LINE, CARRIAGE_RETURN,
                              NEW_LINE, CARRIAGE_RETURN, NEW_LINE},
        std::vector<Token>{
            Token{.value_ = std::vector<char32_t>{CARRIAGE_RETURN, NEW_LINE},
                  .type_ = TokenType::LINE_TERMINATOR,
                  .ignored_ = true,
                  .line_ = 1,
                  .column_ = 1},
            Token{.value_ = std::vector<char32_t>{CARRIAGE_RETURN, NEW_LINE},
                  .type_ = TokenType::LINE_TERMINATOR,
                  .ignored_ = true,
                  .line_ = 2,
                  .column_ = 1},
            Token{.value_ = std::vector<char32_t>{CARRIAGE_RETURN, NEW_LINE},
                  .type_ = TokenType::LINE_TERMINATOR,
                  .ignored_ = true,
                  .line_ = 3,
                  .column_ = 1}})));

class IsTokenTypeIgnoredTestFixture
    : public testing::TestWithParam<std::tuple<TokenType, bool>> {};

TEST_P(IsTokenTypeIgnoredTestFixture, IsTokenTypeIgnored_DetectIgnored) {
  auto [type, ignored] = GetParam();

  ASSERT_EQ(ignored, is_token_type_ignored(type));
}

INSTANTIATE_TEST_SUITE_P(
    IsTokenTypeIgnoredTest, IsTokenTypeIgnoredTestFixture,
    testing::Values(std::make_tuple(TokenType::PUNCTUATOR, false),
                    std::make_tuple(TokenType::NAME, false),
                    std::make_tuple(TokenType::INT_VALUE, false),
                    std::make_tuple(TokenType::FLOAT_VALUE, false),
                    std::make_tuple(TokenType::STRING_VALUE, false),
                    std::make_tuple(TokenType::UNICODE_BOM, true),
                    std::make_tuple(TokenType::WHITESPACE, true),
                    std::make_tuple(TokenType::LINE_TERMINATOR, true),
                    std::make_tuple(TokenType::COMMENT, true),
                    std::make_tuple(TokenType::COMMA, true)));

class TokenizeDetectPunctuatorTestFixture
    : public testing::TestWithParam<
          std::tuple<std::vector<char32_t>, std::vector<Token>>> {};

TEST_P(TokenizeDetectPunctuatorTestFixture,
       IsSourceCharacterAPunctuator_DetectPunctuator) {
  auto [s, expected_tokens] = GetParam();

  Result<std::vector<Token>, TokenizeError> r = tokenize(s);

  ASSERT_TRUE(r.IsOk());

  std::unique_ptr<std::vector<Token>> tokens = r.Unwrap();

  ASSERT_EQ(expected_tokens.size(), tokens->size());

  for (size_t i = 0; i < expected_tokens.size(); i++) {
    ASSERT_EQ(expected_tokens.at(i), tokens->at(i));
  }
}

INSTANTIATE_TEST_SUITE_P(
    PunctuatorTest, TokenizeDetectPunctuatorTestFixture,
    testing::Values(
        std::make_tuple(std::vector{U'!'},
                        std::vector{Token{.value_ = std::vector{U'!'},
                                          .type_ = TokenType::PUNCTUATOR,
                                          .ignored_ = false,
                                          .line_ = 1,
                                          .column_ = 1}}),
        std::make_tuple(std::vector{U'$'},
                        std::vector{Token{.value_ = std::vector{U'$'},
                                          .type_ = TokenType::PUNCTUATOR,
                                          .ignored_ = false,
                                          .line_ = 1,
                                          .column_ = 1}}),
        std::make_tuple(std::vector{U'&'},
                        std::vector{Token{.value_ = std::vector{U'&'},
                                          .type_ = TokenType::PUNCTUATOR,
                                          .ignored_ = false,
                                          .line_ = 1,
                                          .column_ = 1}}),
        std::make_tuple(std::vector{U'('},
                        std::vector{Token{.value_ = std::vector{U'('},
                                          .type_ = TokenType::PUNCTUATOR,
                                          .ignored_ = false,
                                          .line_ = 1,
                                          .column_ = 1}}),
        std::make_tuple(std::vector{U')'},
                        std::vector{Token{.value_ = std::vector{U')'},
                                          .type_ = TokenType::PUNCTUATOR,
                                          .ignored_ = false,
                                          .line_ = 1,
                                          .column_ = 1}}),
        std::make_tuple(std::vector{U'.', U'.', U'.'},
                        std::vector{
                            Token{.value_ = std::vector{U'.', U'.', U'.'},
                                  .type_ = TokenType::PUNCTUATOR,
                                  .ignored_ = false,
                                  .line_ = 1,
                                  .column_ = 1}})));