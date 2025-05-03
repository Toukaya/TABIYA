#ifndef TABIYA_HPP
#define TABIYA_HPP

#include "IterWrapper.hpp"

namespace tabiya {
    template<typename T>
    struct RangeIncrementor {
        explicit RangeIncrementor(bool ascending) : _ascending(ascending) {}
        auto operator()(T& value) const -> decltype(auto) {
            if (_ascending) {
                if constexpr (PrefixDecrementable<T>)
                    return --value;
                else
                    return value--;
            } else {
                if constexpr (PrefixIncrementable<T>)
                    return ++value;
                else
                    return value++;
            }
        }
    private:
        bool _ascending;
    };

    template<typename T>
    struct RangeEqualityComparator {
        explicit RangeEqualityComparator(bool ascending) : _ascending(ascending) {}
        constexpr auto operator()(T& left, T& right) const -> bool {
            return _ascending ? (left <= right) : (left >= right);
        }
        constexpr auto operator()(const T& left, const T& right) const -> bool {
            return _ascending ? (left <= right) : (left >= right);
        }
    private:
        bool _ascending;
    };

    template<
        typename T,
        typename Incrementor = RangeIncrementor<T>,
        typename Dereferencer = DefaultDereferencer<T>,
        typename EqualityComparator = RangeEqualityComparator<T>
    >
    requires GreaterThanComparable<T> && LessThanComparable<T> && InequalityComparable<T> && Bidirectional<T>
    class Range {
    public:
        using Iter = IterWrapper<T, Incrementor, Dereferencer, EqualityComparator>;

        Range() = default;

        Range& from(T start) {
            _startVal = start;
            return *this;
        }

        Range& to(T end) {
            _ascending = (_startVal >= end);
            _endVal = end;
            return *this;
        }

        Iter begin() const { return makeIter(_startVal); }
        Iter end()   const { return makeIter(_endVal);   }

    private:
        T    _startVal{};
        T    _endVal{};
        bool _ascending{true};

        [[nodiscard]] Incrementor makeInc() const {
            if constexpr (IsInstanceOf<RangeIncrementor, Incrementor>)
                return Incrementor{_ascending};
            else {
                static_assert(std::is_default_constructible_v<Incrementor>);
                return Incrementor{};
            }
        }

        [[nodiscard]] EqualityComparator makeEq() const {
            if constexpr (IsInstanceOf<RangeEqualityComparator, EqualityComparator>)
                return EqualityComparator{_ascending};
            else {
                static_assert(std::is_default_constructible_v<EqualityComparator>);
                return EqualityComparator{};
            }
        }

        [[nodiscard]] Iter makeIter(T pos) const {
            return Iter(pos, makeInc(), Dereferencer{}, makeEq());
        }

    };
} // namespace tabiya

#endif // TABIYA_HPP