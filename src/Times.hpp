//
// Created by Touka on 7/21/24.
//

#ifndef TIMES_HPP
#define TIMES_HPP

#include "IterWrapper.hpp"
#include "concepts/concept.hpp"

namespace tabiya {

    template<GreaterEqualityComparable T>
    struct TimesEqualityComparator;

    template <typename T>
    class Times {
    public:
        explicit Times(T count) : _count(count) {
            if constexpr (IsSigned<T>) {
                if (count < 0) {
                    _count = 0;
                }
            }
        }

        using Iter = IterWrapper<T, DefaultIncrementor<T>, decltype([](T value) {return value;}), TimesEqualityComparator<T>>;

        Iter begin() const {
            return Iter{T(0)};
        }

        Iter end() const {
            return Iter{_count};
        }

    private:
        T _count;
    };

    template<GreaterEqualityComparable T>
    struct TimesEqualityComparator {
        constexpr bool operator()(T& left, T& right) const {
            return left >= right;
        }
        constexpr bool operator()(const T& left, const T& right) const {
            return left >= right;
        }
    };

} // tabiya

#endif //TIMES_HPP
