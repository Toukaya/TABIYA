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
    requires Dereferenceable<T> || Numeric<T>
    struct DefaultDereferencer {
        constexpr auto operator()(T &value) const -> decltype(auto) {
            if constexpr (Dereferenceable<T>) {
                return *value;
            } else if constexpr (Numeric<T>) {
                return value;
            }
        }
        constexpr auto operator()(const T &value) const -> decltype(auto) {
            if constexpr (Dereferenceable<T>) {
                return *value;
            } else if constexpr (Numeric<T>) {
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
        using difference_type = std::ptrdiff_t;

        explicit IterWrapper(T position) requires (
                std::default_initializable<Incrementor> &&
                std::default_initializable<Dereferencer> &&
                std::default_initializable<EqualityComparator>)
        : _position(position) {
            if constexpr (!IsInstanceOf<DefaultIncrementor, Incrementor>)
                _incrementor = Incrementor{};
            if constexpr (!IsInstanceOf<DefaultDereferencer, Dereferencer>)
                _dereferencer = Dereferencer{};
            if constexpr (!IsInstanceOf<DefaultEqualityComparator, EqualityComparator>)
                _equalityComparator = EqualityComparator{};
        }

        template<
                typename Inc  = Incrementor,
                typename Deref = Dereferencer,
                typename EqCmp = EqualityComparator
        >
        [[maybe_unused]] explicit IterWrapper(T position,Inc&&  inc,Deref&& der,EqCmp&& eq)
                : _position(position),
                  _incrementor(std::forward<Inc>(inc)),
                  _dereferencer(std::forward<Deref>(der)),
                  _equalityComparator(std::forward<EqCmp>(eq)) {}

        auto operator*() -> decltype(auto) requires Dereferenceable<T> {
            if constexpr (IsInstanceOf<DefaultDereferencer, Dereferencer>) {
                return *_position;
            } else {
                return _dereferencer(_position);
            }
        }

        auto operator*() -> decltype(auto) requires (not Dereferenceable<T>) {
            return _dereferencer(_position);
        }

        auto operator*() const -> decltype(auto) requires Dereferenceable<T> {
            if constexpr (IsInstanceOf<DefaultDereferencer, Dereferencer>) {
                return *_position;
            } else {
                return _dereferencer(_position);
            }
        }

        auto operator*() const -> decltype(auto) requires (not Dereferenceable<T>) {
            return _dereferencer(_position);
        }

        auto operator++() -> decltype(*this) {
            if constexpr (IsInstanceOf<DefaultIncrementor, Incrementor>) {
                ++_position;
            } else {
                _incrementor(_position);
            }
            return *this;
        }

        bool operator!=(const IterWrapper& other) const {
            if constexpr (IsInstanceOf<DefaultEqualityComparator, EqualityComparator>) {
                return _position != other._position;
            } else {
                return not _equalityComparator(_position, other._position);
            }
        }

        auto next() -> decltype(++*this) {
            return ++(*this);
        }

        auto source() -> decltype(auto) {
            return *(*this);
        }

        auto source() const -> decltype(auto) {
            return *(*this);
        }

    private:
        T _position;
        Incrementor _incrementor;
        Dereferencer _dereferencer;
        EqualityComparator _equalityComparator;
    };
} // tabiya

#endif //ITERWRAPPER_HPP
