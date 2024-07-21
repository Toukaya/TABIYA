//
// Created by Touka on 7/21/24.
//

#ifndef ITERWRAPPER_HPP
#define ITERWRAPPER_HPP

#include "concepts/concept.hpp"

namespace tabiya {

    // Default incrementor
    template<Addable T>
    struct DefaultIncrementor {
        void operator()(T& position) const { ++position; }
    };

    // Default dereferencer
    template<Dereferenceable T>
    struct DefaultDereferencer {
        auto operator()(T& position) const -> decltype(*position) { return *position; }
    };

    // Default comparator
    template<Equalable T>
    struct DefaultComparator {
        bool operator()(const T& lhs, const T& rhs) const { return lhs != rhs; }
    };

    template<
        typename T,
        typename Incrementor = DefaultIncrementor<T>,
        typename Dereferencer = DefaultDereferencer<T>,
        typename Comparator = DefaultComparator<T>
    >
    class IterWrapper {
    public:
        explicit IterWrapper(T position) : position(position) {}

        IterWrapper(T position, Incrementor inc, Dereferencer deref, Comparator cmp)
            : position(position), incrementor(inc), dereferencer(deref), comparator(cmp) {}

        T operator*() {
            return dereferencer(position);
        }

        IterWrapper& operator++() {
            incrementor(position);
            return *this;
        }

        bool operator!=(const IterWrapper& other) const {
            return comparator(position, other.position);
        }
    private:
        T position;
        Incrementor incrementor;
        Dereferencer dereferencer;
        Comparator comparator;
    };
} // tabiya

#endif //ITERWRAPPER_HPP
