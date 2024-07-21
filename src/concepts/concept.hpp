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
    concept Equalable = requires(T a, T b) {
        { a == b } -> std::convertible_to<bool>;
        { a != b } -> std::convertible_to<bool>;
    };

    template<typename T>
    concept IsSigned = std::signed_integral<T>;
} // tabiya

#endif //CONCEPT_HPP
