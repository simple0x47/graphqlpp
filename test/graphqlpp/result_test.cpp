// Copyright (c) Gabriel Amihalachioaie, SimpleG 2024.

#include "graphqlpp/result.h"

#include <gtest/gtest.h>

using namespace graphqlpp;

class NoDefaultConstructor {
 public:
  explicit NoDefaultConstructor(int x) {}
};

TEST(ResultTest, IsOk_ReturnsTrueForOkValue) {
  const Result<int, bool> r = Result<int, bool>::Ok(120);

  ASSERT_TRUE(r.IsOk());
}

TEST(ResultTest, IsOk_ReturnsFalseForErrValue) {
  const Result<int, bool> r = Result<int, bool>::Err(false);

  ASSERT_FALSE(r.IsOk());
}

TEST(ResultTest, Ok_PointerCompatible) {
  std::string example = "hey";
  const Result<std::string*, bool> r = Result<std::string*, bool>::Ok(&example);

  ASSERT_TRUE(r.IsOk());
}

TEST(ResultTest, Err_PointerCompatible) {
  const Result<std::string*, bool> r = Result<std::string*, bool>::Err(false);

  ASSERT_FALSE(r.IsOk());
}

TEST(ResultTest, Ok_NoDefaultConstructorWorks) {
  const Result<bool, NoDefaultConstructor> r =
      Result<bool, NoDefaultConstructor>::Ok(false);

  ASSERT_TRUE(r.IsOk());
}

TEST(ResultTest, Err_NoDefaultConstructorWorks) {
  const Result<NoDefaultConstructor, bool> r =
      Result<NoDefaultConstructor, bool>::Err(false);

  ASSERT_FALSE(r.IsOk());
}

TEST(ResultTest, Unwrap_ReturnsUniquePtrWithOkValue) {
  Result<std::string, bool> r = Result<std::string, bool>::Ok("yes");

  const std::unique_ptr<std::string> ok = r.Unwrap();

  ASSERT_EQ("yes", *ok);
}

TEST(ResultTest, Unwrap_ThrowsExceptionForErrorValue) {
  Result<std::string, bool> r = Result<std::string, bool>::Err(false);

  ASSERT_THROW({ auto v = r.Unwrap(); }, std::logic_error);
}

TEST(ResultTest, UnwrapErr_ReturnsUniquePtrWithErrorValue) {
  Result<std::string, bool> r = Result<std::string, bool>::Err(true);

  const std::unique_ptr<bool> error = r.UnwrapErr();

  ASSERT_TRUE(error);
}

TEST(ResultTest, UnwrapErr_ThrowsExceptionForOkValue) {
  Result<std::string, bool> r = Result<std::string, bool>::Ok("yes");

  ASSERT_THROW({ auto e = r.UnwrapErr(); }, std::logic_error);
}