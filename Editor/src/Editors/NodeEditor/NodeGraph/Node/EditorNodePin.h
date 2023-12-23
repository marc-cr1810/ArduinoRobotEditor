#pragma once

#include "ImGui/ImGuiCommon.h"
#include "Core/UUID.h"

enum class PinType
{
    Invalid = -1,
    Generic,
    Execution,

    // Value Types
    Bool,
    String,
    Float,
    Float2,
    Float3,
    Float4
};

static ImColor GetPinColor(PinType type)
{
    switch (type)
    {
    case PinType::Invalid: return ImColor(1.0f, 0.0f, 0.0f, 1.0f);
    case PinType::Generic: return ImColor(1.0f, 1.0f, 1.0f, 1.0f);
    case PinType::Execution: return ImColor(1.0f, 1.0f, 1.0f, 1.0f);
    case PinType::Bool: return ImColor(239, 35, 60);
    case PinType::String: return ImColor(156, 81, 182);
    case PinType::Float:
    case PinType::Float2:
    case PinType::Float3:
    case PinType::Float4: return ImColor(184, 216, 186);
    default:
        ED_ASSERT(0, "Not implemented");
        break;
    }
    return ImColor(0, 0, 0, 255);
}

static bool IsGenericPinType(PinType startPinType, PinType endPinType)
{
    return endPinType == PinType::Generic && startPinType >= PinType::Bool && startPinType <= PinType::Float4;
}

struct EditorNodePin
{
    bool IsInput = false;
    PinType Type = PinType::Invalid;
    PinID ID = 0;
    std::string Label = "";

    static const std::string ToString(PinType pinType);

    static EditorNodePin CreateInputPin(const std::string& label, PinType type);
    static EditorNodePin CreateOutputPin(const std::string& label, PinType type);
};