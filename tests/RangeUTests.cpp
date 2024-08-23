//
// Created by Touka on 7/22/24.
//
#include <catch2/catch_all.hpp>
#include "../src/Range.hpp"
#include <vector>
#include <string>

SCENARIO("Range iterates over a range of integers correctly") {
    GIVEN("A Range instance with a start and end range of integers") {

        WHEN("Iterated over") {
            std::vector<int> results;
            for (auto it: tabiya::Range<int>{}.from(1).to(4)) {
                results.push_back(it);
            }

            THEN("The results contain all numbers in the range") {
                REQUIRE(results == std::vector<int>{1, 2, 3});
            }
        }
    }
}

SCENARIO("Range handles empty ranges correctly") {
    GIVEN("A Range instance with the same start and end") {
        tabiya::Range<int> emptyRange;
        emptyRange.from(5).to(5);

        WHEN("Checked if the range is empty by comparing begin and end iterators") {
            bool isEmpty = !(emptyRange.begin() != emptyRange.end());

            THEN("The range is considered empty") {
                REQUIRE(isEmpty);
            }
        }
    }
}

SCENARIO("Range works with custom incrementors") {
    GIVEN("A Range instance with a custom incrementor that increments by 2") {
        struct CustomIncrementor {
            void operator()(int& value) const { value += 2; }
        };

        tabiya::Range<int, CustomIncrementor> customRange;
        customRange.from(0).to(5); // Should include 0, 2, 4

        WHEN("Iterated over") {
            std::vector<int> results;
            for(auto it = customRange.begin(); it != customRange.end(); ++it) {
                results.push_back(*it);
            }

            THEN("The results are incremented by 2 each step") {
                REQUIRE(results == std::vector<int>{0, 2, 4});
            }
        }
    }
}

SCENARIO("Range supports reverse iteration with a custom decrementor") {
    GIVEN("A Range instance with a custom decrementor that decrements by 1") {
        struct CustomDecrementor {
            void operator()(int& value) const { value -= 1; }
        };

        tabiya::Range<int, CustomDecrementor> reverseRange;
        reverseRange.from(3).to(0); // Should include 3, 2, 1

        WHEN("Iterated over in reverse") {
            std::vector<int> results;
            for(auto it = reverseRange.begin(); it != reverseRange.end(); ++it) {
                results.push_back(*it);
            }

            THEN("The results are decremented by 1 each step") {
                REQUIRE(results == std::vector<int>{3, 2, 1});
            }
        }
    }
}