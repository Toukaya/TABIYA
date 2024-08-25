//
// Created by Touka on 7/21/24.
//

#ifndef ITERWRAPPER_HPP
#define ITERWRAPPER_HPP

#include "concepts/concept.hpp"

namespace tabiya {

#pragma region Default Concepts

    template<typename  T>
    requires PrefixIncrementable<T> || PostfixIncrementable<T>
    struct DefaultIncrementor final {
        auto operator()(T& value) const -> decltype(auto) {
            if constexpr (PrefixIncrementable<T>)
                return ++value;
            else
                return value++;
        }
    };

    template<typename T>
    requires Dereferenceable<T> || IsIntegral<T>
    struct DefaultDereferencer {
        constexpr auto operator()(T &value) const -> decltype(auto) {
            if constexpr (Dereferenceable<T>) {
                return *value;
            } else if constexpr (IsIntegral<T>) {
                return value;
            }
        }
    };

    template<EqualityComparable T>
    struct DefaultEqualityComparator final {
        constexpr auto operator()(T& left, T& right) const -> bool { return left == right; }
    };

    template <template <typename...> class Template, typename >
    constexpr bool IsInstanceOf = false;

    template <template <typename...> class Template, typename... Args>
    constexpr bool IsInstanceOf<Template, Template<Args...>> = true;

#pragma endregion

    template<
        typename T,
        typename Incrementor = DefaultIncrementor<T>,
        typename Dereferencer = DefaultDereferencer<T>,
        typename EqualityComparator = DefaultEqualityComparator<T>
    >
    requires std::is_invocable_v<Incrementor, T&> &&
             std::is_invocable_v<Dereferencer, T&> &&
             std::is_invocable_v<EqualityComparator, T&, T&>
    class IterWrapper {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::remove_cvref_t<decltype(Dereferencer{}(std::declval<T&>()))>;
        using pointer = value_type*;
        using reference = value_type&;

        explicit IterWrapper(T position) : _position(position) {}

        auto operator*() -> decltype(auto) requires Dereferenceable<T> {
            if constexpr (IsInstanceOf<DefaultDereferencer, Dereferencer>) {
                return *_position;
            } else {
                return Dereferencer{}(_position);
            }
        }

        auto operator*() -> decltype(auto) requires (not Dereferenceable<T>) {
            return Dereferencer{}(_position);
        }

        auto operator++() -> decltype(*this) {
            if constexpr (IsInstanceOf<DefaultIncrementor, Incrementor>) {
                ++_position;
            } else {
                Incrementor{}(_position);
            }
            return *this;
        }

        bool operator!=(const IterWrapper& other) const {
            if constexpr (IsInstanceOf<DefaultEqualityComparator, EqualityComparator>) {
                return _position != other._position;
            } else {
                return not EqualityComparator{}(_position, other._position);
            }
        }

        auto next() -> decltype(++*this) {
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
