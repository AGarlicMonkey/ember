#if EMBER_CORE_ENABLE_PROFILING
    #include <Tracy.hpp>

    #define EMBER_INTERNAL_PROFILER_CONCAT(a, b) EMBER_INTERNAL_PROFILER_CONCAT_2(a, b)
    #define EMBER_INTERNAL_PROFILER_CONCAT_2(a, b) a##b

    #define EMBER_PROFILE_FRAME FrameMark
    #define EMBER_PROFILE_SCOPE(name) ZoneNamedN(EMBER_INTERNAL_PROFILER_CONCAT(_ember_zone_marker_,__LINE__), name, true)
#else
    #define EMBER_PROFILE_FRAME
    #define EMBER_PROFILE_SCOPE(name)
#endif

#define EMBER_PROFILE_FUNCTION EMBER_PROFILE_SCOPE(__FUNCTION__)
