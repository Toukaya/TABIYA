//
// Created by Touka on 7/21/24.
//

#ifndef TIMES_HPP
#define TIMES_HPP

#include "IterWrapper.hpp"
#include "concepts/concept.hpp"

namespace tabiya {
    template<typename T>
    requires GreaterEqualityComparable<T>
    struct TimesEqualityComparator {
        constexpr auto operator()(T& left, T& right) const -> bool {
            return left >= right;
        }
        constexpr auto operator()(const T& left, const T& right) const -> bool {
            return left >= right;
        }
    };

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

        auto begin() const -> Iter {
            return Iter{T(0)};
        }

        auto end() const -> Iter {
            return Iter{_count};
        }

    private:
        T _count;
    };
} // tabiya

#endif //TIMES_HPP
