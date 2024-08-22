//
// Created by Touka on 7/22/24.
//

#ifndef TABIYA_HPP
#define TABIYA_HPP
#include "IterWrapper.hpp"

namespace tabiya {
    template<
        typename T,
        typename Incrementor = DefaultIncrementor<T>,
        typename Dereferencer = DefaultDereferencer<T>,
        typename Comparator = DefaultComparator<T>
    >
    class Range {
    public:

        using Iter = IterWrapper<T, Incrementor, Dereferencer, Comparator>;

        auto from(T start) -> decltype(*this) {
            _curr = Iter(start);
            return *this;
        }

        auto to(T end) -> decltype(*this) {
            _end = Iter(end);
            return *this;
        }

        auto begin() const -> Iter {
            return _curr;
        }

        auto end() const -> Iter {
            return _end;
        }

    private:
        Iter _curr;
        Iter _end;
    };
} // tabiya

#endif //TABIYA_HPP
