

#pragma once

#include <type_traits>


// Credits to "StoryTeller"
// https://stackoverflow.com/questions/50086607/check-which-base-classes-are-implemented/

template<template<typename...> class TT, class T>
struct is_specialization_base_of {
    template<typename... Args>
    static constexpr std::true_type  check(TT<Args...> const&);

    static constexpr std::false_type check(...);

    static constexpr bool value = decltype(check(std::declval<T>()))::value;

    template<typename... Args>
    static constexpr TT<Args...>  check2(TT<Args...> const&);

    static constexpr void check2(...);

    using base_type = decltype(check2(std::declval<T>()));
};






/*





class A {};
template <typename T, typename S> class B : public virtual A {};
template <typename T> class C : public virtual A {};
class D : public B<int, int> {};
class E : public C<int> {};
class F : public D, E {};

template <typename T>
void foo() {
    using transition_t = typename is_specialization_base_of<B, T>::base_type;

    static_assert(is_specialization_base_of<B, T>::value);
    static_assert(std::is_same<transition_t, void>::value);
    //static_assert(std::is_same<transition_t, B<int>>::value);
}

void bar() {
    foo<F>;
}*/