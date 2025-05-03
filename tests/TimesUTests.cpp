//
// Created by Touka on 7/21/24.
//
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "../src/Times.hpp"
#include <numeric>
#include <vector>
#include <string>

SCENARIO("Times class creates proper sequence of integer values", "[Times]") {
    GIVEN("A Times object with positive integer count") {
        tabiya::Times<int> times(5);

        WHEN("Iterating through the values") {
            std::vector<int> collected;
            for (auto value : times) {
                collected.push_back(value);
            }

            THEN("The sequence contains values from 0 to count-1") {
                REQUIRE(collected.size() == 5);
                REQUIRE(collected[0] == 0);
                REQUIRE(collected[1] == 1);
                REQUIRE(collected[2] == 2);
                REQUIRE(collected[3] == 3);
                REQUIRE(collected[4] == 4);
            }
        }

        WHEN("Using standard algorithms") {
            auto sum = std::accumulate(times.begin(), times.end(), 0);

            THEN("The sum matches expected arithmetic progression") {
                // Sum of 0 + 1 + 2 + 3 + 4 = 10
                REQUIRE(sum == 10);
            }
        }
    }
}

SCENARIO("Times handles different numeric types", "[Times]") {
    GIVEN("A Times object with unsigned int") {
        tabiya::Times<unsigned int> times(3);

        WHEN("Collecting the sequence") {
            std::vector<unsigned int> values;
            for (auto val : times) {
                values.push_back(val);
            }

            THEN("The sequence is correct for unsigned type") {
                REQUIRE(values.size() == 3);
                REQUIRE(values == std::vector<unsigned int>{0, 1, 2});
            }
        }
    }

    GIVEN("A Times object with double type") {
        tabiya::Times<double> times(3.5);

        WHEN("Iterating through values") {
            std::vector<double> values;
            for (auto val : times) {
                values.push_back(val);
            }

            THEN("The sequence contains correct number of doubles") {
                REQUIRE(values.size() == 4); // Should be truncated to 4
                REQUIRE(values[0] == 0.0);
                REQUIRE(values[1] == 1.0);
                REQUIRE(values[2] == 2.0);
                REQUIRE(values[3] == 3.0);
            }
        }
    }
}

SCENARIO("Times handles edge cases", "[Times]") {
    GIVEN("A Times object with zero count") {
        tabiya::Times<int> times(0);

        WHEN("Iterating through values") {
            std::vector<int> values;
            for (auto val : times) {
                values.push_back(val);
            }

            THEN("The sequence is empty") {
                REQUIRE(values.empty());
            }
        }
    }

    GIVEN("A Times object with negative count") {
        tabiya::Times<int> times(-5);

        WHEN("Iterating through values") {
            std::vector<int> values;
            for (auto val : times) {
                values.push_back(val);
            }

            THEN("The sequence is empty due to negative-to-zero conversion") {
                REQUIRE(values.empty());
            }
        }
    }

    GIVEN("A very large Times count") {
        tabiya::Times<size_t> times(1000);

        WHEN("Testing begin and end iterators") {
            auto begin = times.begin();
            auto end = times.end();

            THEN("Begin points to 0 and end points to count") {
                REQUIRE(*begin == 0);
                REQUIRE(*end == 1000); // NOTE: It can be seen that unlike the end of the general std container, the end of times can be dereferenced
            }
        }
    }
}

SCENARIO("Times iterators support standard operations", "[Times]") {
    GIVEN("A Times object") {
        tabiya::Times<int> times(10);
        auto begin = times.begin();
        auto end = times.end();

        WHEN("Using iterator operations") {
            auto it = begin;
            ++it;
            ++it;

            THEN("The iterator advances correctly") {
                REQUIRE(*it == 2);
            }

            WHEN("Comparing iterators") {
                auto it2 = begin;
                ++it2;
                ++it2;

                THEN("Equal iterators compare as equal") {
                    REQUIRE(!(it != it2));
                }

                THEN("Different iterators compare as not equal") {
                    ++it2;
                    REQUIRE(it != it2);
                }
            }
        }

        WHEN("Using iterator in standard algorithms") {
            auto sum = std::accumulate(begin, end, 0);
            auto count = std::count_if(begin, end, [](int n) { return n % 2 == 0; });

            THEN("Algorithms produce expected results") {
                // Sum of 0 to 9 = 45
                REQUIRE(sum == 45);
                // Even numbers in [0,9] = 5 (0,2,4,6,8)
                REQUIRE(count == 5);
            }
        }
    }
}

SCENARIO("Times with custom types that meet requirements", "[Times]") {
    // Create a custom numeric type that satisfies the requirements
    struct CustomNumber {
        int value;

        explicit CustomNumber(int val) : value(val) {}

        CustomNumber operator+(const CustomNumber& other) const {
            return CustomNumber(value + other.value);
        }

        CustomNumber &operator++() {
            ++value;
            return *this;
        }

        bool operator>=(const CustomNumber& other) const {
            return value >= other.value;
        }
    };

    GIVEN("A Times object with custom numeric type") {
        tabiya::Times times(CustomNumber(3));

        WHEN("Iterating through values") {
            std::vector<int> values;
            for (auto val : times) {
                values.push_back(val.value);
            }

            THEN("The sequence contains expected values") {
                REQUIRE(values.size() == 3);
                REQUIRE(values[0] == 0);
                REQUIRE(values[1] == 1);
                REQUIRE(values[2] == 2);
            }
        }
    }
}