// Copyright (c) Gabriel Amihalachioaie, SimpleG 2024.

#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "../../result.h"
#include "token.h"
#include "tokenize_error.h"

namespace graphqlpp::language::tokenization {
constexpr char32_t NEW_LINE = U'\U0000000A';
constexpr char32_t CARRIAGE_RETURN = U'\U0000000D';

/// \brief GraphQL source text to tokens.
/// \param source GraphQL source text.
/// \return A vector of <i>Token</i>s or an <i>TokenizeError</i>.
Result<std::vector<Token>, TokenizeError> tokenize(
    const std::vector<char32_t>& source);

/// \brief Detect whether or not a character is a valid source character.
/// \param source_character Character to be tested.
/// \return True if the character is a valid source character, false otherwise.
bool is_source_character_valid(char32_t source_character);

/// \brief Return whether or not the type is an ignored token.
/// \param type Type of the token.
/// \return True if the token type is an ignored token, false otherwise.
bool is_token_type_ignored(TokenType type);

}  // namespace graphqlpp::language::tokenization

#endif  // TOKENIZER_H