#pragma once

// 3rdparty lib includes
#include <cpptypesafeenum.h>

template<typename T>
struct iterateEnum;

#define DECLARE_BOBBYTYPESAFE_ENUM(Name, Derivation, Values) \
    DECLARE_TYPESAFE_ENUM(Name, Derivation, Values) \
    \
    template<> \
    struct iterateEnum<Name> { \
        template<typename T> \
        static void iterate(T&&cb) \
        { \
            return iterate##Name(std::forward<T>(cb)); \
        } \
    };
