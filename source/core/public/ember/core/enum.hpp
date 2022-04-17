#pragma once

#define EMBER_ENUM_BIT_FLAG_OPERATORS(enum_type, integral_type)                                      \
    inline enum_type constexpr operator&(enum_type l, enum_type r) {                                  \
        return static_cast<enum_type>(static_cast<integral_type>(l) & static_cast<integral_type>(r)); \
    }                                                                                              \
    inline enum_type constexpr operator|(enum_type l, enum_type r) {                                  \
        return static_cast<enum_type>(static_cast<integral_type>(l) | static_cast<integral_type>(r)); \
    }                                                                                              \
    inline enum_type constexpr operator^(enum_type l, enum_type r) {                                  \
        return static_cast<enum_type>(static_cast<integral_type>(l) ^ static_cast<integral_type>(r)); \
    }                                                                                              \
    inline enum_type constexpr operator&=(enum_type &l, enum_type r) {                                \
        l = l & r;                                                                                 \
        return l;                                                                                  \
    }                                                                                              \
    inline enum_type constexpr operator|=(enum_type &l, enum_type r) {                                \
        l = l | r;                                                                                 \
        return l;                                                                                  \
    }                                                                                              \
    inline enum_type constexpr operator^=(enum_type &l, enum_type r) {                                \
        l = l ^ r;                                                                                 \
        return l;                                                                                  \
    }                                                                                              \
    inline bool constexpr operator==(enum_type l, integral_type r) {                                 \
        return static_cast<integral_type>(l) == r;                                                  \
    }                                                                                              \
    inline bool constexpr operator!=(enum_type l, integral_type r) {                                 \
        return !(l == r);                                                                          \
    }
