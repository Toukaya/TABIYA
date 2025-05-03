//
// Created by Touka on 7/22/24.
//
#include <catch2/catch_all.hpp>
#include "../src/Range.hpp"
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <numeric>

SCENARIO("Range iterates over basic integer ranges") {
    GIVEN("A Range from 1 to 5") {
        tabiya::Range<int> range;
        range.from(1).to(5);

        WHEN("Iterating over the range") {
            std::vector<int> results;
            for (auto value : range) {
                results.push_back(value);
            }

            THEN("The results contain all numbers from 1 to 4") {
                REQUIRE(results == std::vector<int>{1, 2, 3, 4});
            }
        }
    }

    GIVEN("A Range from 5 to 1 (descending)") {
        tabiya::Range<int> range;
        range.from(5).to(1);

        WHEN("Iterating over the range") {
            std::vector<int> results;
            for (auto value : range) {
                results.push_back(value);
            }

            THEN("The results contain all numbers from 5 to 2 in descending order") {
                REQUIRE(results == std::vector<int>{5, 4, 3, 2});
            }
        }
    }
}

SCENARIO("Range works with different numeric types") {
    GIVEN("A Range of double values") {
        tabiya::Range<double> range;
        range.from(1.5).to(4.5);

        WHEN("Iterating over the range") {
            std::vector<double> results;
            for (auto value : range) {
                results.push_back(value);
            }

            THEN("The results contain values starting at 1.5 and incrementing by 1.0") {
                REQUIRE(results.size() == 3);
                REQUIRE(results[0] == (1.5));
                REQUIRE(results[1] == (2.5));
                REQUIRE(results[2] == (3.5));
            }
        }
    }

    GIVEN("A Range of unsigned integers") {
        tabiya::Range<unsigned int> range;
        range.from(10).to(15);

        WHEN("Iterating over the range") {
            std::vector<unsigned int> results;
            for (auto value : range) {
                results.push_back(value);
            }

            THEN("The results contain all numbers from 10 to 14") {
                REQUIRE(results == std::vector<unsigned int>{10, 11, 12, 13, 14});
            }
        }
    }
}

SCENARIO("Range handles edge cases correctly") {
    GIVEN("A Range where start equals end") {
        tabiya::Range<int> range;
        range.from(5).to(5);

        WHEN("Checking if iteration produces any values") {
            std::vector<int> results;
            for (auto value : range) {
                results.push_back(value);
            }

            THEN("No values are produced") {
                REQUIRE(results.empty());
            }
        }
    }

    GIVEN("A Range with start greater than end") {
        tabiya::Range<int> range;
        range.from(10).to(5);

        WHEN("Iterating over the range") {
            std::vector<int> results;
            for (auto value : range) {
                results.push_back(value);
            }

            THEN("The values are produced in descending order") {
                REQUIRE(results == std::vector<int>{10, 9, 8, 7, 6});
            }
        }
    }

    GIVEN("A default-constructed Range") {
        tabiya::Range<int> range;

        WHEN("Iterating over the range without setting bounds") {
            std::vector<int> results;
            int count = 0;
            for (auto value : range) {
                if (count++ > 10) break; // Safety limit
                results.push_back(value);
            }

            THEN("It starts from the default value (0)") {
                REQUIRE(results.empty());
            }
        }
    }
}

SCENARIO("Range works with custom incrementor and dereferencer") {
    GIVEN("A Range with custom incrementor that increments by 2") {
        struct CustomIncrementor {
            auto operator()(int& value) const -> decltype(auto) {
                return value += 2;
            }
        };

        tabiya::Range<int, CustomIncrementor> range;
        range.from(1).to(10);

        WHEN("Iterating over the range") {
            std::vector<int> results;
            for (auto value : range) {
                results.push_back(value);
            }

            THEN("Values increment by 2 each step") {
                REQUIRE(results == std::vector<int>{1, 3, 5, 7, 9});
            }
        }
    }

    GIVEN("A Range with custom dereferencer that doubles the value") {
        struct DoubleValueDereferencer {
            auto operator()(int& value) const -> int {
                return value * 2;
            }
        };

        tabiya::Range<int, tabiya::DefaultIncrementor<int>, DoubleValueDereferencer> range;
        range.from(1).to(5);

        WHEN("Iterating over the range") {
            std::vector<int> results;
            for (auto &&value : range) {
                results.push_back(value);
            }

            THEN("Each value is doubled") {
                REQUIRE(results == std::vector<int>{2, 4, 6, 8});
            }
        }
    }
}

SCENARIO("Range integrates with STL algorithms") {
    GIVEN("A Range of integers") {
        tabiya::Range<int> range;
        range.from(1).to(10);

        WHEN("Used with std::accumulate") {
            int sum = std::accumulate(range.begin(), range.end(), 0);

            THEN("The sum is calculated correctly") {
                REQUIRE(sum == 45); // 1+2+3+4+5+6+7+8+9 = 45
            }
        }

        WHEN("Used with std::count_if to count even numbers") {
            int count = std::count_if(range.begin(), range.end(), [](int n) { return n % 2 == 0; });

            THEN("The count is correct") {
                REQUIRE(count == 4); // 2, 4, 6, 8 are even
            }
        }
    }
}

//SCENARIO("Range works with custom types that satisfy the requirements") {
//    struct CustomType {
//        int value;
//
//        explicit CustomType(int v);
//        explicit CustomType();
//
//        bool operator<(const CustomType& other) const { return value < other.value; }
//        bool operator>(const CustomType& other) const { return value > other.value; }
//        bool operator==(const CustomType& other) const { return value == other.value; }
//        int operator*() const { return value; }
//
//        CustomType& operator++() { ++value; return *this; }
//    };
//
//    GIVEN("A Range of CustomType") {
//        tabiya::Range<CustomType> range;
//        range.from(CustomType(1)).to(CustomType(5));
//
//        WHEN("Iterating over the range") {
//            std::vector<int> results;
//            for (auto ct : range) {
//                results.push_back(ct);
//            }
//
//            THEN("The values are processed correctly") {
//                REQUIRE(results == std::vector<int>{1, 2, 3, 4});
//            }
//        }
//    }
//}

SCENARIO("Range can be used to create sequences for containers") {
    GIVEN("A Range from 1 to 5") {
        tabiya::Range<int> range;
        range.from(1).to(5);

        WHEN("Used to initialize a vector") {
            std::vector<int> vec(range.begin(), range.end());

            THEN("The vector contains all values in the range") {
                REQUIRE(vec == std::vector<int>{1, 2, 3, 4});
            }
        }

        WHEN("Used to initialize a set") {
            std::set<int> set(range.begin(), range.end());

            THEN("The set contains all values in the range") {
                REQUIRE(set.size() == 4);
                REQUIRE(set.find(1) != set.end());
                REQUIRE(set.find(2) != set.end());
                REQUIRE(set.find(3) != set.end());
                REQUIRE(set.find(4) != set.end());
            }
        }
    }
}

SCENARIO("Range can represent reversed ranges") {
    GIVEN("A Range from 5 to 1") {
        tabiya::Range<int> range;
        range.from(5).to(1);

        WHEN("Iterating over the range") {
            std::vector<int> results;
            for (auto value : range) {
                results.push_back(value);
            }

            THEN("Values are produced in descending order") {
                REQUIRE(results == std::vector<int>{5, 4, 3, 2});
            }
        }
    }
}

SCENARIO("Range works with large integer ranges") {
    GIVEN("A Range with a large span") {
        tabiya::Range<int> range;
        range.from(1).to(10000);

        WHEN("Checking specific values") {
            auto begin = range.begin();
            auto value1 = *begin;

            // Advance to the 100th element
            auto it = begin;
            for (int i = 0; i < 99; ++i) ++it;
            auto value100 = *it;

            // Check the 1000th element
            it = begin;
            for (int i = 0; i < 999; ++i) ++it;
            auto value1000 = *it;

            THEN("Values at specific positions are correct") {
                REQUIRE(value1 == 1);
                REQUIRE(value100 == 100);
                REQUIRE(value1000 == 1000);
            }
        }
    }
}

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

SCENARIO("Range works with custom incrementors") {
    GIVEN("A Range instance with a custom incrementor that increments by 2") {
        struct CustomIncrementor {
            void operator()(int& value) const { value += 2; }
        };

        tabiya::Range<int, CustomIncrementor> customRange;
        customRange.from(0).to(5); // Should include 0, 2, 4

        WHEN("Iterated over") {
            std::vector<int> results;
            for(auto &&it : customRange) {
                results.push_back(it);
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
            for(auto && it : reverseRange) {
                results.push_back(it);
            }

            THEN("The results are decremented by 1 each step") {
                REQUIRE(results == std::vector<int>{3, 2, 1});
            }
        }
    }
}