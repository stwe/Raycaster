#pragma once

#include "Log.h"

#if defined(_WIN64) && defined(_MSC_VER)
    #define FPS_DEBUG_BREAK __debugbreak()
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 12)
    #include <csignal>
    #define FPS_DEBUG_BREAK raise(SIGTRAP)
#else
    #error Unsupported platform or compiler!
#endif

#ifdef FPS_DEBUG_BREAK
    #define FPS_ENABLE_ASSERTS
#endif

#ifdef FPS_ENABLE_ASSERTS
    #define FPS_ASSERT(x, ...)                                   \
    {                                                                \
    if (!(x))                                                        \
    {                                                                \
    FPS_LOG_ERROR("Assertion Failed: {0}, File: {1}, Line: {2}", \
    __VA_ARGS__, __FILE__, __LINE__);                                \
    FPS_DEBUG_BREAK;                                             \
    }                                                                \
    }
#else
    #define FPS_ASSERT(x, ...)
#endif
