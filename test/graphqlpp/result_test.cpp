// Copyright (c) Gabriel Amihalachioaie, SimpleG 2024.

#include "graphqlpp/result.h"

#include <gtest/gtest.h>

using namespace graphqlpp;

class NoDefaultConstructor {
 public:
  explicit NoDefaultConstructor(int x) {}
};

TEST(ResultTest, IsOk_ReturnsTrueForOkValue) {
  const Result<int, bool> r = Result<int, bool>::ok(120);

  ASSERT_TRUE(r.isOk());
}

TEST(ResultTest, IsOk_ReturnsFalseForErrValue) {
  const Result<int, bool> r = Result<int, bool>::err(false);

  ASSERT_FALSE(r.isOk());
}

TEST(ResultTest, Ok_PointerCompatible) {
  std::string example = "hey";
  const Result<std::string*, bool> r = Result<std::string*, bool>::ok(&example);

  ASSERT_TRUE(r.isOk());
}

TEST(ResultTest, Err_PointerCompatible) {
  const Result<std::string*, bool> r = Result<std::string*, bool>::err(false);

  ASSERT_FALSE(r.isOk());
}

TEST(ResultTest, Ok_NoDefaultConstructorWorks) {
  const Result<bool, NoDefaultConstructor> r =
      Result<bool, NoDefaultConstructor>::ok(false);

  ASSERT_TRUE(r.isOk());
}

TEST(ResultTest, Err_NoDefaultConstructorWorks) {
  const Result<NoDefaultConstructor, bool> r =
      Result<NoDefaultConstructor, bool>::err(false);

  ASSERT_FALSE(r.isOk());
}

TEST(ResultTest, Unwrap_ReturnsUniquePtrWithOkValue) {
  Result<std::string, bool> r = Result<std::string, bool>::ok("yes");

  const std::unique_ptr<std::string> ok = r.unwrap();

  ASSERT_EQ("yes", *ok);
}

TEST(ResultTest, Unwrap_ThrowsExceptionForErrorValue) {
  Result<std::string, bool> r = Result<std::string, bool>::err(false);

  ASSERT_THROW({ auto v = r.unwrap(); }, std::logic_error);
}

TEST(ResultTest, UnwrapErr_ReturnsUniquePtrWithErrorValue) {
  Result<std::string, bool> r = Result<std::string, bool>::err(true);

  const std::unique_ptr<bool> error = r.unwrapErr();

  ASSERT_TRUE(error);
}

TEST(ResultTest, UnwrapErr_ThrowsExceptionForOkValue) {
  Result<std::string, bool> r = Result<std::string, bool>::ok("yes");

  ASSERT_THROW({ auto e = r.unwrapErr(); }, std::logic_error);
}