//
// Created by Touka on 7/21/24.
//

#ifndef TIMES_HPP
#define TIMES_HPP
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

        class Iterator {
        public:
            explicit Iterator(T position) : _position(position) {
            }

            T operator*() const {
                return _position;
            }

            Iterator& operator++() {
                ++_position;
                return *this;
            }

            bool operator!=(const Iterator&other) const {
                return _position != other._position;
            }

        private:
            T _position;
        };

        Iterator begin() const {
            return Iterator(0);
        }

        Iterator end() const {
            return Iterator(_count);
        }

    private:
        T _count;
    };
} // tabiya

#endif //TIMES_HPP
