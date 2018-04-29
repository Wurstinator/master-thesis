

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


template<template<typename...> class TT, class T>
struct specialization_base_of {

    template<typename... Args>
    static constexpr TT<Args...>  checkVal(TT<Args...> const&);

    static constexpr void checkVal(...);

    template<typename... Args>
    static constexpr TT<Args...>&  checkRef(TT<Args...> const&);

    static constexpr void checkRef(...);

    template<typename... Args>
    static constexpr TT<Args...> const& checkCref(TT<Args...> const&);

    static constexpr void checkCref(...);

    template<typename... Args>
    static constexpr TT<Args...>*  checkPtr(TT<Args...> const&);

    static constexpr void checkPtr(...);

    using value_type = decltype(checkVal(std::declval<T>()));
    using ref_type = decltype(checkRef(std::declval<T>()));
    using cref_type = decltype(checkCref(std::declval<T>()));
    using ptr_type = decltype(checkPtr(std::declval<T>()));
};

