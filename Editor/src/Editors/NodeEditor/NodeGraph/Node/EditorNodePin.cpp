#include "epch.h"
#include "EditorNodePin.h"

const std::string EditorNodePin::ToString(PinType pinType)
{
	switch (pinType)
	{
	case PinType::Invalid: return "Invalid";
	case PinType::Execution: return "Execution";
	case PinType::Bool: return "Bool";
	case PinType::Float: return "Float";
	case PinType::Float2: return "Float2";
	case PinType::Float3: return "Float3";
	case PinType::Float4: return "Float4";
	default: ED_ASSERT(0, "Not implemented!");
	}
	return "<unknown>";
}

EditorNodePin EditorNodePin::CreateInputPin(const std::string& label, PinType type)
{
	EditorNodePin pin;
	pin.IsInput = true;
	pin.Type = type;
	pin.ID = UUID();
	pin.Label = label;
	return pin;
}

EditorNodePin EditorNodePin::CreateOutputPin(const std::string& label, PinType type)
{
	EditorNodePin pin;
	pin.IsInput = false;
	pin.Type = type;
	pin.ID = UUID();
	pin.Label = label;
	return pin;
}
