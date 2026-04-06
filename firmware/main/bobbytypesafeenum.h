#pragma once

// 3rd.party lib includes
#include <cpptypesafeenum.h>

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
