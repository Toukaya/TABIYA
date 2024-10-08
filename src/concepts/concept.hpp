//
// Created by Touka on 7/21/24.
//

#ifndef CONCEPT_HPP
#define CONCEPT_HPP

namespace tabiya {
    template<typename T>
    concept Addable = requires(T a, T b) {
        { a + b } -> std::convertible_to<T>;
    };

    template<typename T>
    concept Subtractable = requires(T a, T b) {
        { a - b } -> std::convertible_to<T>;
    };

    template<typename T>
    concept AddSubtractable = Addable<T> && Subtractable<T>;

    template<typename T>
    concept EqualityComparable = requires(T a, T b) {
        { a == b } -> std::same_as<bool>;
    };

    template<typename T>
    concept InequalityComparable = requires(T a, T b) {
        { a != b } -> std::same_as<bool>;
    };

    template<typename T>
    concept Equalable = EqualityComparable<T> && InequalityComparable<T>;

    template<typename T>
    concept Dereferenceable = requires(T a) {
            { *a };
    };

    template<typename T>
    concept PrefixIncrementable = requires(T a) {
        { ++a } -> std::same_as<T&>;
    };

    template<typename T>
    concept PostfixIncrementable = requires(T a) {
        { a++ } -> std::same_as<T>;
    };

    template<typename T>
    concept Numeric = std::is_arithmetic_v<T>;

    template<typename T>
    concept IsSigned = std::signed_integral<T>;

    template<typename T>
    concept IsUnsigned = std::unsigned_integral<T>;

    template<typename T>
    concept IsIntegral = std::is_integral_v<T>;

} // tabiya

#endif //CONCEPT_HPP
