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

// Mock concepts and templates for testing purposes
struct Mock {
    int value;
    explicit Mock(int v) : value(v) {}
    Mock& operator++() { ++value; return *this; }
    Mock operator++(int) { Mock temp = *this; ++value; return temp; }
    bool operator==(const Mock& other) const { return value == other.value; }
    int& operator*() { return value; }
};

struct CustomIncrementor {
    void operator()(Mock& obj) const { obj.value += 2; }
};

struct CustomDereferencer {
    Mock& operator()(Mock& obj) const { return obj; }
};

TEST_CASE("Test Default IterWrapper with MockIncrementable", "[iterwrapper]") {
    using namespace tabiya;

    Mock start(0);
    Mock end(5);

    IterWrapper it(start);
    IterWrapper it_end(end);

    SECTION("Increment Test") {
        REQUIRE(it != it_end);
        ++it;
        REQUIRE(it.source() == 1);
    }

    SECTION("Dereference Test") {
        REQUIRE((*it) == 0);
        ++it;
        REQUIRE(it.source() == 1);
    }
}

TEST_CASE("Test Custom Incrementor and Dereferencer", "[iterwrapper]") {
    using namespace tabiya;

    Mock start(0);
    Mock deref(10);

    IterWrapper<Mock, CustomIncrementor> it(start);
    IterWrapper<Mock, DefaultIncrementor<Mock>, CustomDereferencer> deref_it(deref);

    SECTION("Custom Incrementor Test") {
        ++it;
        REQUIRE(it.source() == 2);
    }

    SECTION("Custom Dereferencer Test") {
        REQUIRE(*deref_it == deref);
    }
}

TEST_CASE("Test with Standard Containers", "[iterwrapper]") {
    using namespace tabiya;

    std::vector vec = {1, 2, 3, 4};
    IterWrapper it(vec.begin());
    IterWrapper it_end(vec.end());

    SECTION("Increment Test with Vector") {
        REQUIRE(it != it_end);
        ++it;
        REQUIRE(*it == 2);
    }

    SECTION("Dereference Test with Vector") {
        REQUIRE(*it == 1);
        ++it;
        REQUIRE(*it == 2);
    }
}

#pragma region algorithm
TEST_CASE("Testing IterWrapper with std algorithms and ranges", "[IterWrapper]") {
    using namespace tabiya;

    // Create a vector and wrap its iterator
    std::vector vec{1, 2, 3, 4, 5};
    IterWrapper iter_begin(vec.begin());
    IterWrapper iter_end(vec.end());

    SECTION("Testing std::accumulate with IterWrapper") {
        int sum = std::accumulate(iter_begin, iter_end, 0);
        REQUIRE(sum == 15); // 1+2+3+4+5 = 15
    }

    SECTION("Testing std::find with IterWrapper") {
        auto found = std::find(iter_begin, iter_end, 3);
        REQUIRE(*found == 3);
    }
}

#pragma endregion

int main(int argc, char* argv[]) {
    Catch::Session session;

    int result = session.run(argc, argv);

    return result;
}