#pragma once

#if EMBER_EXPORT
    #ifdef _MSC_VER
        #ifdef EMBER
            #define EMBER_API __declspec(dllexport)
        #else
            #define EMBER_API __declspec(dllimport)
        #endif
    #else
        #error ember currently only supported on MSVC
    #endif
#else
    #define EMBER_API
#endif