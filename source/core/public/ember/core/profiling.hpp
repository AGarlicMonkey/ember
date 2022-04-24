#if EMBER_CORE_ENABLE_PROFILING
    #include <Tracy.hpp>

    #define EMBER_PROFILE_FRAME FrameMark
    #define EMBER_PROFILE_SCOPE(name) ZoneScopedN(name)
#else
    #define EMBER_PROFILE_FRAME
    #define EMBER_PROFILE_SCOPE(name)
#endif

#define EMBER_PROFILE_FUNCTION EMBER_PROFILE_SCOPE(__FUNCTION__)
