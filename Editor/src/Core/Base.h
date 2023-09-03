#pragma once

#include <glm/glm.hpp>
#include <memory>

#ifdef ED_DEBUG
#define ED_DEBUGBREAK() __debugbreak()
#define ED_ENABLE_ASSERTS
#else
#define ED_DEBUGBREAK()
#endif

#define ED_EXPAND_MACRO(x) x
#define ED_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

using Float2 = glm::vec2;
using Float3 = glm::vec3;
using Float4 = glm::vec4;

#include "Core/Log.h"
#include "Core/Assert.h"