//
// Created by Touka on 7/21/24.
//

#ifndef ITERWRAPPER_HPP
#define ITERWRAPPER_HPP

#include "concepts/concept.hpp"

namespace tabiya {

#pragma region Default Concepts
    struct NoConstructor { NoConstructor() = delete; };

    struct NoDestructor { ~NoDestructor() = delete; };

    template<typename  T>
    requires PrefixIncrementable<T> || PostfixIncrementable<T>
    struct DefaultIncrementor final : NoConstructor, NoDestructor {
        decltype(auto) operator()(T& value) const { return ++value; }
    };

    template<Dereferenceable T>
    struct DefaultDereferencer final : NoConstructor, NoDestructor {
        decltype(auto) operator()(T& value) const { return *value; }
    };

    template<Equalable T>
    struct DefaultComparator final : NoConstructor, NoDestructor {
        decltype(auto) operator()(T& value, T& other) const { return value != other; }
    };

    template <template <typename> class , typename >
    struct is_instance_of : std::false_type {};

    template <template <typename> class Temp, typename T>
    struct is_instance_of<Temp, Temp<T>> : std::true_type {};

    template <template <typename> class Temp, typename T>
    inline constexpr bool is_instance_of_v = is_instance_of<Temp, T>::value;

#pragma endregion

    template<
        typename T,
        typename Incrementor = DefaultIncrementor<T>,
        typename Dereferencer = DefaultDereferencer<T>,
        typename Comparator = DefaultComparator<T>
    >
    requires std::is_invocable_v<Incrementor, T&> &&
             std::is_invocable_v<Dereferencer, T&> &&
             std::is_invocable_v<Comparator, T&, T&>
    class IterWrapper {
    public:
        explicit IterWrapper(T position) : _position(position) {}

        decltype(auto) operator*() {
            if constexpr (is_instance_of_v<DefaultDereferencer, Dereferencer>) {
                return *_position;
            } else {
                return Dereferencer{}(_position);
            }
        }

        IterWrapper& operator++() {
            if constexpr (is_instance_of_v<DefaultIncrementor, Incrementor>) {
                ++_position;
            } else {
                Incrementor{}(_position);
            }
            return *this;
        }

        bool operator!=(const IterWrapper& other) const {
            if constexpr (is_instance_of_v<DefaultComparator, Comparator>) {
                return _position != other._position;
            } else {
                return Comparator{}(_position, other._position);
            }
        }

        auto next() -> IterWrapper& {
            return ++(*this);
        }

        auto source() -> decltype(auto) {
            return *(*this);
        }

    private:
        T _position;
    };
} // tabiya

#endif //ITERWRAPPER_HPP
