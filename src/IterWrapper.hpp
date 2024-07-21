//
// Created by Touka on 7/21/24.
//

#ifndef ITERWRAPPER_HPP
#define ITERWRAPPER_HPP

#include "concepts/concept.hpp"

namespace tabiya {

    template<Addable>
    struct DefaultIncrementor final {
        DefaultIncrementor() = delete;
        ~DefaultIncrementor() = delete;
    };

    template<Dereferenceable>
    struct DefaultDereferencer final {
        DefaultDereferencer() = delete;
        ~DefaultDereferencer() = delete;
    };

    template<Equalable>
    struct DefaultComparator final {
        DefaultComparator() = delete;
        ~DefaultComparator() = delete;
    };

    template <template <typename> class Temp, typename T>
    struct is_instance_of : std::false_type {};

    template <template <typename> class Temp, typename T>
    struct is_instance_of<Temp, Temp<T>> : std::true_type {};

    template <template <typename> class Temp, typename T>
    inline constexpr bool is_instance_of_v = is_instance_of<Temp, T>::value;

    template<
        typename T,
        typename Incrementor = DefaultIncrementor<T>,
        typename Dereferencer = DefaultDereferencer<T>,
        typename Comparator = DefaultComparator<T>
    >
    class IterWrapper {
    public:
        explicit IterWrapper(T position) : position(position) {}

        auto operator*() {
            if constexpr (is_instance_of_v<DefaultDereferencer, Dereferencer>) {
                return *position;
            } else {
                return Dereferencer{}(position);
            }
        }

        IterWrapper& operator++() {
            if constexpr (is_instance_of_v<DefaultIncrementor, Incrementor>) {
                ++position;
            } else {
                Incrementor{}(position);
            }
            return *this;
        }

        bool operator!=(const IterWrapper& other) const {
            if constexpr (is_instance_of_v<DefaultComparator, Comparator>) {
                return position != other.position;
            } else {
                return Comparator{}(position, other.position);
            }
        }
    private:
        T position;
    };
} // tabiya

#endif //ITERWRAPPER_HPP
