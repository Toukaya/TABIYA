//
// Created by Touka on 7/21/24.
//

#include <catch2/catch_test_macros.hpp>
#include "../src/Times.hpp"

using namespace tabiya;

TEST_CASE("Times with positive count iterates correctly", "[Times]") {
    int count = 0;
    for (auto time :  Times(5)) {
        ++count;
    }
    REQUIRE(count == 5);
}

TEST_CASE("Times with zero count iterates zero times", "[Times]") {
    int count = 0;
    for (auto time : Times(0)) {
        ++count;
    }
    REQUIRE(count == 0);
}

TEST_CASE("Times iterator returns correct values", "[Times]") {
    int expectedValue = 0;
    for (auto time : Times(3)) {
        REQUIRE(time == expectedValue);
        ++expectedValue;
    }
}

TEST_CASE("Times with negative count behaves as zero count", "[Times]") {
    int count = 0;
    for (auto time : Times(-1)) {
        ++count;
    }
    REQUIRE(count == 0);
}

TEST_CASE("Times with negative count behaves as zero count using unsign int", "[Times]") {
    int count = 0;
    for (auto time :  Times(5u)) {
        ++count;
    }
    REQUIRE(count == 5);
}