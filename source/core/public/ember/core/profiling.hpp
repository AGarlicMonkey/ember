#pragma once

#if EMBER_CORE_ENABLE_PROFILING
    #include <Tracy.hpp>

namespace ember::inline core::internal {
    constexpr std::uint32_t rgb_to_32(float const r, float const g, float const b, float const a) {
        std::uint32_t const colour{ std::uint32_t(0xff * r) << 16 |
                                    std::uint32_t(0xff * g) << 8 |
                                    std::uint32_t(0xff * b) };
        return colour * a;//a is only ever 0 or 1
    }
}

    #define EMBER_INTERNAL_PROFILER_CONCAT(a, b) EMBER_INTERNAL_PROFILER_CONCAT_2(a, b)
    #define EMBER_INTERNAL_PROFILER_CONCAT_2(a, b) a##b

    #define EMBER_PROFILE_FRAME FrameMark
    #define EMBER_PROFILE_SCOPE(name) ZoneNamedN(EMBER_INTERNAL_PROFILER_CONCAT(_ember_zone_marker_, __LINE__), name, true)
    #define EMBER_PROFILE_SCOPE_C(name, r, g, b) ZoneNamedNC(EMBER_INTERNAL_PROFILER_CONCAT(_ember_zone_marker_, __LINE__), name, ::ember::core::internal::rgb_to_32(r, g, b, 1.0f), true)
#else
    #define EMBER_PROFILE_FRAME
    #define EMBER_PROFILE_SCOPE(name)
    #define EMBER_PROFILE_SCOPE_C(name, r, g, b)
#endif

#define EMBER_PROFILE_FUNCTION EMBER_PROFILE_SCOPE(__FUNCTION__)
