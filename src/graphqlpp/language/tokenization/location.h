// Copyright (c) Gabriel Amihalachioaie, SimpleG 2024.

#ifndef LOCATION_H
#define LOCATION_H

namespace graphqlpp::language::tokenization {
/// \brief Location, within a GraphQL, script where an error occurred.
struct Location {
  /// \brief Line of the error.
  size_t line_;
  /// \brief Column of the error.
  size_t column_;
};
}  // namespace graphqlpp::language::tokenization

#endif  // LOCATION_H
