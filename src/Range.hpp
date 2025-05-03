#ifndef TABIYA_HPP
#define TABIYA_HPP

#include "IterWrapper.hpp"

namespace tabiya {
    template<
        typename T,
        typename Incrementor = DefaultIncrementor<T>,
        typename Dereferencer = DefaultDereferencer<T>
    >
    requires GreaterThanComparable<T> && LessThanComparable<T> && InequalityComparable<T>
    class Range {
    public:
        using Iter = IterWrapper<T, Incrementor, Dereferencer>;

        class Sentinel {
        public:
            constexpr Sentinel(T end, bool ascending) 
                : _end(end), _ascending(ascending) {}

            friend bool operator!=(const Iter& it, const Sentinel& sen)  {
                const auto curr = *it;
                return sen._ascending ? (curr < sen._end)
                                       : (curr > sen._end);
            }
        private:
            T     _end;
            bool  _ascending;
        };

        Range() = default;

        Range& from(T start) {
            _begin = Iter(start);
            _startVal = start;
            return *this;
        }
        Range& to(T end) {
            _endVal = end;
            _ascending = (_endVal >= _startVal);
            return *this;
        }

        Iter     begin() const { return _begin; }
        Sentinel end()   const { return Sentinel{_endVal, _ascending}; }

    private:
        Iter _begin{T{}};
        T    _startVal{};
        T    _endVal{};
        bool _ascending{true};
    };
} // namespace tabiya

#endif // TABIYA_HPP