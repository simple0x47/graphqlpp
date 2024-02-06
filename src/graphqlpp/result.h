// Copyright (c) Gabriel Amihalachioaie, SimpleG 2024.

#ifndef RESULT_H
#define RESULT_H
#include <memory>

/// \brief Rust's Result type. Indicates whether an operation was a success or a
/// failure by containing two possible values.
/// Its inner value can only be accessed once since the value's ownership
/// is moved to the caller.
/// \tparam Ok Type of the value related to the success of the operation.
/// \tparam Error Type of the value related to the failure of the operation.
template <typename Ok, typename Error>
class Result {
 public:
  /// \brief Whether the contained value is related to a success or a failure.
  /// \return True if the contained value is ok, false otherwise.
  [[nodiscard]] bool isOk() const { return is_ok_; }

  /// \brief Extracts the <b>Ok</b> value from the result. Throws an exception
  /// if the result does not contain a successful value.
  /// \return The <b>Ok</b> value from the result.
  [[nodiscard]] std::unique_ptr<Ok> unwrap() {
    if (!this->isOk()) {
      throw std::logic_error("Unwrapped 'Ok' when the value was an 'Error'.");
    }

    return std::move(this->ok_);
  }

  /// \brief Extracts the <b>Error</b> value from the result. Throws an
  /// exception if the result does not contain a successful value.
  /// \return The <b>Error</b> value from the result.
  [[nodiscard]] std::unique_ptr<Error> unwrapErr() {
    if (this->isOk()) {
      throw std::logic_error("Unwrapped 'Error' when the value was an 'Ok'.");
    }

    return std::move(this->error_);
  }

  /// \brief Creates a successful result containing an <b>Ok</b> value.
  /// \param o Ok value which is moved into an <i>unique_ptr</i>.
  /// \return Result containing the <b>Ok</b> value.
  static Result ok(Ok o) {
    return Result(std::make_unique<Ok>(std::move(o)), std::unique_ptr<Error>(),
                  true);
  }

  /// \brief Creates a failed result containing an <b>Error</b> value.
  /// \param e Error value which is moved into an <i>unique_ptr</i>.
  /// \return Result containing the <b>Error</b> value.
  static Result err(Error e) {
    return Result(std::unique_ptr<Ok>(), std::make_unique<Error>(std::move(e)),
                  false);
  }

 private:
  std::unique_ptr<Ok> ok_;
  std::unique_ptr<Error> error_;
  bool is_ok_;

  Result(std::unique_ptr<Ok> v, std::unique_ptr<Error> e, const bool is_ok) {
    ok_ = std::move(v);
    error_ = std::move(e);
    is_ok_ = is_ok;
  }
};

#endif  // RESULT_H