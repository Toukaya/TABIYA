//
// Created by Touka on 7/21/24.
//

#ifndef TIMES_HPP
#define TIMES_HPP

#include "IterWrapper.hpp"
#include "concepts/concept.hpp"

namespace tabiya {
    template <typename T>
    requires Addable<T> && Equalable<T>
    class Times {
    public:
        explicit Times(T count) : _count(count) {
            if constexpr (IsSigned<T>) {
                if (count < 0) {
                    _count = 0;
                }
            }
        }

        using Iter = IterWrapper<T, DefaultIncrementor<T>, decltype([](T value) {return value;})>;

        auto begin() const -> Iter {
            return Iter{0};
        }

        auto end() const -> Iter {
            return Iter{_count};
        }

    private:
        T _count;
    };
} // tabiya

#endif //TIMES_HPP
