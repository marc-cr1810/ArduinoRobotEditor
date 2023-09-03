#pragma once
#include "Core/Base.h"
#include "Core/Log.h"
#include <filesystem>

#ifdef ED_ENABLE_ASSERTS
// These assert macros are not supposed to be used and are only helpers for ED_ASSERT and ED_CORE_ASSERT

#define ED_INTERNAL_ASSERT_IMPL(check, msg, ...) { if(!(check)) { ED_ERROR(msg, __VA_ARGS__); ED_DEBUGBREAK(); } }
#define ED_INTERNAL_ASSERT_WITH_MSG(check, ...) ED_INTERNAL_ASSERT_IMPL(check, "Assertion failed: {0}", __VA_ARGS__)
#define ED_INTERNAL_ASSERT_NO_MSG(check) ED_INTERNAL_ASSERT_IMPL(check, "Assertion '{0}' failed at {1}:{2}", ED_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LIED__)

#define ED_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define ED_INTERNAL_ASSERT_GET_MACRO(...) ED_EXPAND_MACRO( ED_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, ED_INTERNAL_ASSERT_WITH_MSG, ED_INTERNAL_ASSERT_NO_MSG) )

// These assert macros are supposed to be used, CORE is intended for internal engine stuff

#define ED_ASSERT(...) ED_EXPAND_MACRO( ED_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(__VA_ARGS__) )

#define NOT_IMPLEMENTED ED_ASSERT(0, "Not implemented!")
#else
// Defines them but gives them no use

#define ED_ASSERT(...)
#define ED_CORE_ASSERT(...)
#define NOT_IMPLEMENTED
#endif