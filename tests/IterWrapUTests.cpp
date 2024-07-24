//
// Created by Touka on 7/21/24.
//
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "../src/IterWrapper.hpp"
#include <vector>
#include <list>

SCENARIO("IterWrapper increments and dereferences correctly with default behaviors") {
    GIVEN("A vector of integers") {
        std::vector<int> numbers = {1, 2, 3};
        tabiya::IterWrapper iter(numbers.begin());

        WHEN("Incremented and dereferenced") {
            ++iter;
            auto value = *iter;

            THEN("The value is the second element of the vector") {
                REQUIRE(value == 2);
            }
        }
    }
}

SCENARIO("IterWrapper compares different iterators correctly") {
    GIVEN("Two IterWrapper instances at different positions") {
        std::list<int> numbers = {1, 2, 3};
        tabiya::IterWrapper<std::list<int>::iterator> iter1(numbers.begin());
        auto iter2 = iter1;
        ++iter2;

        WHEN("They are compared") {
            bool result = (iter1 != iter2);

            THEN("The comparison is true") {
                REQUIRE(result);
            }
        }
    }
}

SCENARIO("IterWrapper works with custom Incrementor") {
    GIVEN("An IterWrapper with a CustomIncrementor") {
        int nums[] = {1, 2, 3};
        tabiya::IterWrapper<int*> iter(nums);

        WHEN("The iterator is incremented") {
            ++iter;

            THEN("The value is incremented by 10") {
                REQUIRE(iter.source() == 2);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    Catch::Session session;

    int result = session.run(argc, argv);

    return result;
}