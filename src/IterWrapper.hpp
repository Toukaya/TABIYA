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
        decltype(auto) operator()(T& value) const {
            if constexpr (PrefixIncrementable<T>)
                return ++value;
            else
                return value++;
        }
    };

    template<typename T>
    requires Dereferenceable<T> || Numeric<T>
    struct DefaultDereferencer {
        constexpr decltype(auto) operator()(T &value) const {
            if constexpr (Dereferenceable<T>) {
                return *value;
            } else if constexpr (Numeric<T>) {
                return value;
            }
        }
        constexpr decltype(auto) operator()(const T &value) const {
            if constexpr (Dereferenceable<T>) {
                return *value;
            } else if constexpr (Numeric<T>) {
                return value;
            }
        }
    };

    template<EqualityComparable T>
    struct DefaultEqualityComparator final {
        constexpr bool operator()(T& left, T& right) const { return left == right; }
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
        static constexpr bool UseDefInc = IsInstanceOf<DefaultIncrementor, Incrementor>;
        static constexpr bool UseDefDrf = IsInstanceOf<DefaultDereferencer, Dereferencer>;
        static constexpr bool UseDefCmp = IsInstanceOf<DefaultEqualityComparator, EqualityComparator>;

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
            if constexpr (!UseDefInc) _incrementor = Incrementor{};
            if constexpr (!UseDefDrf) _dereferencer = Dereferencer{};
            if constexpr (!UseDefCmp) _equalityComparator = EqualityComparator{};
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

        decltype(auto) operator*() requires Dereferenceable<T> {
            if constexpr (UseDefDrf) {
                return *_position;
            } else {
                return _dereferencer(_position);
            }
        }

        decltype(auto) operator*() requires (not Dereferenceable<T>) {
            return _dereferencer(_position);
        }

        decltype(auto) operator*() const requires Dereferenceable<T> {
            if constexpr (UseDefDrf) {
                return *_position;
            } else {
                return _dereferencer(_position);
            }
        }

        decltype(auto) operator*() const requires (not Dereferenceable<T>) {
            return _dereferencer(_position);
        }

        auto operator++() -> decltype(*this) {
            if constexpr (UseDefInc) {
                ++_position;
            } else {
                _incrementor(_position);
            }
            return *this;
        }

        bool operator!=(const IterWrapper& other) const {
            if constexpr (UseDefCmp) {
                return _position != other._position;
            } else {
                return not _equalityComparator(_position, other._position);
            }
        }

        auto next() -> decltype(++*this) {
            return ++(*this);
        }

        decltype(auto) source() {
            return *(*this);
        }

        decltype(auto) source() const {
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
