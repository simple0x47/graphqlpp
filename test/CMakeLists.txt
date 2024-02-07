project(graphqlpp_test)

include(FetchContent)

# GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(graphqlpp_test
        graphqlpp/result_test.cpp
        graphqlpp/language/tokenization/tokenizer_test.cpp)

target_link_libraries(graphqlpp_test graphqlpp GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(graphqlpp_test)

include_directories(../src)